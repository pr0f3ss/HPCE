# HPCE: High-Performance Chess Engine

![Chess Engine](https://img.shields.io/badge/chess-engine-brightgreen) ![C++](https://img.shields.io/badge/language-C%2B%2B-blue) ![License](https://img.shields.io/badge/license-MIT-lightgrey)

HPCE is a high-performance chess engine written in C++ that supports:

- **PGN File Streaming**: Parse and manage chess games in the Portable Game Notation (PGN) format.
- **Customizable Chess Rulesets**: Extend and modify piece behavior with an adaptable ruleset.
- **Test Driver**: Comprehensive testing capabilities to validate engine functionality.

## Features

### Core Engine
- **ChessBoard Class** (`HPCE.cpp` / `HPCE.h`):
  - Initializes a chessboard instance.
  - Handles move execution using standard chess notation.
  - Retrieves current game scores.
  - Verifies move legality based on a robust ruleset for each piece.
  - Extensible architecture to modify piece behavior by altering individual rulesets.

### PGN Support
- **PGN_Chess_Game Class** (`PGN_reader.cpp` / `PGN_reader.h`):
  - Stores PGN tag pairs and all game moves.
- **PGN_Reader Class**:
  - Parses PGN files and translates moves into engine-compatible commands.
  - Provides structured access to game metadata and moves.

### Testing
- Includes a **test driver** to validate chess engine operations, ensuring legal move generation, scoring, and PGN parsing integrity.

## Getting Started

### Prerequisites

- **C++ Compiler**: Ensure you have a C++17 (or later) compatible compiler installed.
- **Build Tools**: 
  - `cmake`.

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/pr0f3ss/hpce.git
   cd hpce
   ```

2. Build the project:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

   For Windows MinGW-based systems use:
      ```bash
   mkdir build && cd build
   cmake -G "MinGW Makefiles" ..
   make
   ```

3. Run the tests:
   ```bash
   ./hpce_tests
   ```

## Usage

### Running the Engine

1. Compile the engine:
   ```bash
   make hpce_engine
   ```

2. Execute the engine with a PGN file:
   ```bash
   ./hpce_engine sample_game.pgn
   ```

### Example PGN File
```pgn
[Event "Casual Game"]
[Site "Chess Club"]
[Date "2024.12.15"]
[White "Alice"]
[Black "Bob"]

1. e4 e5 2. Nf3 Nc6 3. Bb5 a6
```

Place your PGN files in the `data/` directory or provide an absolute path.

### Extending Rulesets
Modify the `ChessBoard` implementation in `HPCE.cpp` to add or adjust rules for specific pieces. For example:
- Add custom moves for new chess variants.
- Create unique scoring logic based on specific conditions.

## Project Structure

```
HPCE/
├── src/
│   ├── hpce.cpp                # Core engine implementation
│   ├── hpce.h                  # Core engine header
│   ├── PGN_reader.cpp          # PGN parsing implementation
│   └── PGN_reader.h            # PGN parsing header
├── tests/
│   ├── hpce_test_driver.cpp    # Test driver implementation
│   ├── hpce_test_driver.h      # Test driver header
│   └── hpce_tests.cpp          # Test file runner
├── data/
│   └── test_games.pgn          # Test PGN file
├── CMakeLists.txt              # Build configuration
└── README.md                   # Project documentation
```

## Contributing

Contributions are welcome! Here’s how you can get started:

1. Fork the repository.
2. Create a new branch (`git checkout -b feature/your-feature`).
3. Commit your changes (`git commit -m 'Add your message'`).
4. Push the branch (`git push origin feature/your-feature`).
5. Open a Pull Request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Thanks to the chess and programming community for inspiring this project!

---

### Let’s Play Chess with High Performance!

