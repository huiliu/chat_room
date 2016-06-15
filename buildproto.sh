#!/bin/bash

protoc -Iproto --cpp_out=src/ --python_out=python/ proto/*.proto
