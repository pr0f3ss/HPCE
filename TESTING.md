# 📌 Testing Strategy for PGN Reader

## 📝 Overview
This document outlines the test strategy for the PGN file reader (`return_games()`), which converts PGN files into `PGN_Chess_Game` objects.

## ✅ Testing Framework
- We use **Catch2 v2** for unit testing.
- Tests are located in the `tests/` directory.

## 🧪 Test Categories
We cover various **input variations** to maximize coverage.

| **Category**               | **Variations Covered** |
|----------------------------|----------------------|
| Single-game PGN files      | ✅ Covered |
| Multiple games in a file   | ✅ Partially Covered |
| Missing or malformed tags  | ❌ Missing |
| Invalid move sequences     | ❌ Missing |
| PGN comments (`{}` or `;`) | ❌ Missing |
| Encoding variations        | ❌ Missing |

## 📊 Test Coverage
We measure test case coverage using a script (`test_coverage.py`) that calculates:
```sh 
python tests/test_coverage.py
