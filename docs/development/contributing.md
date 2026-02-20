# Contributing

## Development Setup

1. Clone the repository
2. Build with `make all` or `cmake -B build && cmake --build build`
3. Run tests: `make test`

## Code Style

- C++17
- 4 spaces indent
- Interfaces: pure virtual in `*Hal` / `*Service` headers
- Implementations: in `.cpp` files

## Testing

- Add unit tests in `tests/` for new HAL or services
- Run `make test` before submitting
- Mock drivers used by default

## Documentation

- Update relevant `docs/` when changing APIs
- Regenerate architecture: `python scripts/generate_architecture.py`
- Preview docs: `mkdocs serve`
