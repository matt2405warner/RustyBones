#!/usr/bin/env python

import sys
import clang.cindex
import re
from pathlib import Path
import os
from jinja2 import Environment, FileSystemLoader

clang.cindex.Config.set_library_file('C:/Program Files (x86)/LLVM/bin/libclang.dll')


class Loader(object):
	def __init__(self, path):
		self.path = path
		file_loader = FileSystemLoader(self.path)
		self.env = Environment(loader=file_loader)
		self.env.trim_blocks = True
		self.env.lstrip_blocks = True


class Annotation(object):
	def __init__(self, cursor):
		print("build annotation")
		self.annotations = []
		matches = re.findall("@reflect\s+([A-Za-z0-9()_,\s]+(?=;))+", cursor.raw_comment)
		for m in matches:
			print('Annotation: {}'.format(m))
			self.annotations.append(m)
				

	def serializable(self):
		result = False
		for a in self.annotations:
			if a == "Serialize":
				result = True
				pass
		return result

	def non_serializable(self):
		result = False
		for a in self.annotations:
			if a == "NonSerialize":
				result = True
				pass
		return result

	def __str__(self):
		data = "Annotations:\n"
		for a in self.annotations:
			data += "   " + a + "\n"
		return data


def get_annotations(cursor):
	return [c.displayname for c in cursor.get_children()
			if c.kind == clang.cindex.CursorKind.ANNOTATE_ATTR]


def get_qualifiers(t):
	q = set()
	if t.is_const_qualified():
		q.add('const')
	if t.is_volatile_qualified():
		q.add('volatile')
	if t.is_restrict_qualified():
		q.add('restrict')
	return q


class Variable(object):
	def __init__(self, cursor):
		self.name = cursor.spelling
		self.type = cursor.type.spelling
		self.annotations = get_annotations(cursor)
		self.serializable = (cursor.access_specifier == clang.cindex.AccessSpecifier.PUBLIC)
		for a in self.annotations:
			if a == "Serializable":
				self.serializable = True
				break
			elif a == "NonSerializable":
				self.serializable = False
				break
		self.qualifiers = set()
		if self.serializable:
			self.qualifiers = get_qualifiers(cursor.type)

	def __str__(self):
		data = "Variable Name: {}\n".format(self.name)
		data += "   Type: {}\n".format(self.type)
		for a in self.annotations:
			data += "   Annotations: {}\n".format(a)
		for q in self.qualifiers:
			data += "   Qualifiers: {}\n".format(q)
		return data


class Function(object):
	def __init__(self, cursor):
		self.name = cursor.spelling
		self.annotations = get_annotations(cursor)
		self.serializable = (cursor.access_specifier == clang.cindex.AccessSpecifier.PUBLIC)
		for a in self.annotations:
			if a == "NonSerializable":
				self.serializable = False
				break
			elif a == "Serializable":
				self.serializable = True
				break

	def __str__(self):
		data = "Function Name: {}\n".format(self.name)
		for a in self.annotations:
			data += "   Annotation: {}\n".format(a)
		return data


class Class(object):
	def __init__(self, cursor, annotations):
		self.name = cursor.spelling
		self.functions = []
		self.members = []
		self.annotations = annotations

		for c in cursor.get_children():
			if c.kind == clang.cindex.CursorKind.CXX_METHOD:
				f = Function(c)
				if f.serializable:
					self.functions.append(f)
			elif c.kind == clang.cindex.CursorKind.FIELD_DECL:
				v = Variable(c)
				if v.serializable:
					self.members.append(v)

	def __str__(self):
		data = ""
		data += "Class Name: {}\n".format(self.name)
		data += str(self.annotations)
		for f in self.functions:
			data += str(f)
		for m in self.members:
			data += str(m)
		return data


def build_classes(cursor, loc):
	result = []
	for c in cursor.get_children():
		print('cursor: {}, {}'.format(c.displayname, c.kind))
		if c.kind == clang.cindex.CursorKind.CLASS_DECL and c.location.file.name == loc:
			print('Found class')
			if not c.raw_comment:
				print("class has no comment")
				continue
			annotation = Annotation(c)
			can_serialize = annotation.serializable()
			if can_serialize:
				print("class serializable")
				a_class = Class(c, annotation)
				result.append(a_class)
		elif c.kind == clang.cindex.CursorKind.NAMESPACE:
			child_classes = build_classes(c, loc)
			result.extend(child_classes)

	return result


def main():
	index = clang.cindex.Index.create()
	directory = os.path.dirname(os.path.realpath(__file__))
	classlist = []
	includes = []

	# Path through each header file with extension ending in '.hpp'
	pathlist = Path(directory).glob('**/*.hpp')
	for path in pathlist:
		filename = str(path)
		tu = index.parse(filename)
		print('Translation unit: ', tu.spelling)
		classes = build_classes(tu.cursor, filename)
		for cls in classes:
			print(cls)
		if len(classes) > 0:
			relpath = str(path.relative_to(directory))
			print(relpath)
			includes.append(relpath)
		classlist.extend(classes)

	loader = Loader('')

	# Generate Header
	template_header = loader.env.get_template('./Meta/class_meta_generator_header.txt')
	output_header = template_header.render(includes=includes, classes=classlist)
	meta_generated_file_h = './Meta/meta.generated.h'
	with open(meta_generated_file_h, "w+") as f:
		f.write(output_header)

	# Generate Source
	#meta_generate_file_s = 'meta.generated.cpp'
	#template_source = loader.env.get_template('class_meta_generator_source.txt')
	#pch_name = 'Core.Engine.h'
	#output_source = template_source.render(pch=pch_name, gen_name=meta_generated_file_h)
	#with open(meta_generate_file_s, "w+") as f:
	#	f.write(output_source)


if __name__ == "__main__":
	main()
