# Running Tests

## Make

```bash
make test
```

## CMake

```bash
cmake -B build -DBUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

## Direct Binary

```bash
./build/streaming_device_tests
```

## Expected Output

```
=== HAL Tests ===
Test: Display init ... OK
...
========================================
Passed: 44, Failed: 0
```
