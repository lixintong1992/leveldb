#!/usr/bin/env bash

find ./ -maxdepth 1 | grep -Fxv "./" | grep -v "build.sh" | grep -v "clean.sh" | xargs rm -rf

