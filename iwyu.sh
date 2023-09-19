#!/usr/bin/env bash

if [ "$1" == "check" ]; then
	iwyu_tool.py -p=./compile_commands.json | tee ./iwyu_output
	exit 0
fi
if [ "$1" == "fix" ]; then
	fix_includes.py <./iwyu_output
	exit 0
fi
