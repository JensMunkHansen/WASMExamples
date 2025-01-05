#!/bin/bash
node scripts/run_wasm_test_file.js --wasm build/example.wasm --input ./test_files/input.json --expected ./test_files/expected_output.json
