# This file generates a notation vector for the hpce_tests.cpp file.

def parse_moves(notation):
    moves = notation.replace("\n", " ").split()
    results = []
    for move in moves:
        if "." in move:
            move_number = int(move.split(".")[0])
            results.append((move_number, 0, move.split('.', 1)[-1]))
        else:
            results.append((move_number, 1, move))
            
    return results

# Input list of moves
notation = """
1.e4 c5 2.Nf3 a6 3.d3 g6 4.g3 Bg7 5.Bg2 b5 6.O-O Bb7 7.c3 e5 8.a3 Ne7 9.b4 d6
10.Nbd2 O-O 11.Nb3 Nd7 12.Be3 Rc8 13.Rc1 h6 14.Nfd2 f5 15.f4 Kh7 16.Qe2 cxb4
17.axb4 exf4 18.Bxf4 Rxc3 19.Rxc3 Bxc3 20.Bxd6 Qb6+ 21.Bc5 Nxc5 22.bxc5 Qe6
23.d4 Rd8 24.Qd3 Bxd2 25.Nxd2 fxe4 26.Nxe4 Nf5 27.d5 Qe5 28.g4 Ne7 29.Rf7+ Kg8
30.Qf1 Nxd5 31.Rxb7 Qd4+ 32.Kh1 Rf8 33.Qg1 Ne3 34.Re7 a5 35.c6 a4 36.Qxe3 Qxe3
37.Nf6+ Rxf6 38.Rxe3 Rd6 39.h4 Rd1+ 40.Kh2 b4 41.c7 1-0
"""

# Generate the list of objects
parsed_moves = parse_moves(notation)

# Output the results
for obj in parsed_moves:
    print(f"{{{obj[0]}, {obj[1]}, \"{obj[2]}\"}},")
