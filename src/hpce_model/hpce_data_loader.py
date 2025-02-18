import os
import torch
from torch.utils.data import Dataset, DataLoader
import hpce
from concurrent.futures import ThreadPoolExecutor

class ChessDataset(Dataset):
    def __init__(self, pgn_dir):
        self.pgn_dir = pgn_dir
        self.pgn_reader = hpce.PGN_Reader()
        self.chess_board = hpce.Chess_Board()
        self.games = self._load_games()

    def _load_games(self):
        """Load all PGN files in parallel and return a list of games."""
        pgn_files = [os.path.join(self.pgn_dir, f) for f in os.listdir(self.pgn_dir) if f.endswith(".pgn")]
        games = []
        
        def process_file(pgn_file):
            """Read games from a PGN file."""
            return self.pgn_reader.return_games(pgn_file)
        
        # Use ThreadPoolExecutor to parallelize PGN read in
        with ThreadPoolExecutor() as executor:
            results = list(executor.map(process_file, pgn_files))
        
        for result in results:
            games.extend(result)
        
        return games

    def __len__(self):
        return len(self.games)

    def __getitem__(self, idx):
        curr_game = self.games[idx]
        input_sequence = self.chess_board.get_input_sequence(curr_game).board_tokens
        return torch.tensor([input_sequence], dtype=torch.float32)

if __name__ == "__main__":
    training_dir = "../../training_data/"
    dataset = ChessDataset(training_dir)
    dataloader = DataLoader(dataset, batch_size=2, shuffle=True, num_workers=4)

    for batch in dataloader:
        print(batch)
