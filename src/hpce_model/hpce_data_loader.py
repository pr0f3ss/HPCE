import torch
from torch.utils.data import Dataset, DataLoader
import pgn_reader

class ChessDataset(Dataset):
    def __init__(self, pgn_file):
        self.pgn_file = pgn_file
        self.pgn_reader = pgn_reader.PGN_Reader()
        self.games = self.pgn_reader.return_games(pgn_file)

    def __len__(self):
        return len(self.games)

    # TODO: Implement item retrieval
    def __getitem__(self, idx):
        curr_game = self.games[idx]
        # return torch.tensor([ord(c) for c in position], dtype=torch.float32)

if __name__ == "__main__":
    pgn_file = "../../data/pass_test_case.pgn"
    dataset = ChessDataset(pgn_file)
    dataloader = DataLoader(dataset, batch_size=2, shuffle=True)

    for batch in dataloader:
        print(batch)