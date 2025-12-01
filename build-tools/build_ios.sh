#!/bin/bash
cd "$(dirname "$0")/.."
scons platform=ios arch=arm64 target=template_release
