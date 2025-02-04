import torch
from torch.utils.data import Dataset, DataLoader
import hpce_pgn_reader

class ChessDataset(Dataset):
    def __init__(self, pgn_file):
        self.pgn_file = pgn_file
        self.positions = hpce_pgn_reader.process_pgn(pgn_file)

    def __len__(self):
        return len(self.positions)

    def __getitem__(self, idx):
        position = self.positions[idx]
        # Convert the position to a tensor (replace with your actual preprocessing logic)
        # Example: Dummy tensor for demonstration
        return torch.tensor([ord(c) for c in position], dtype=torch.float32)

# Example usage
if __name__ == "__main__":
    pgn_file = "example.pgn"
    dataset = ChessDataset(pgn_file)
    dataloader = DataLoader(dataset, batch_size=2, shuffle=True)

    for batch in dataloader:
        print(batch)