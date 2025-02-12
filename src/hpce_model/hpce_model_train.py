import torch.optim as optim
import torch.nn as nn
from hpce_data_loader import ChessDataset
from torch.utils.data import DataLoader
from hpce_model import ChessTransformer

# Initialize model, optimizer, and loss functions
model = ChessTransformer(token_dim=112)
optimizer = optim.Adam(model.parameters(), lr=1e-4)
move_loss_fn = nn.CrossEntropyLoss()
eval_loss_fn = nn.MSELoss()

pgn_file = "../../data/pass_test_case.pgn"
dataset = ChessDataset(pgn_file)
dataloader = DataLoader(dataset, batch_size=2, shuffle=True)

n_epochs = 10

for epoch in range(n_epochs): 
    for batch in dataloader:  # TODO: implement data loader
        input_tensor, move_targets, eval_targets = batch
        
        # Forward pass
        move_logits, eval_score = model(input_tensor)
        
        # Compute losses
        move_loss = move_loss_fn(move_logits.view(-1, 64 * 64), move_targets.view(-1))
        eval_loss = eval_loss_fn(eval_score.squeeze(), eval_targets)
        total_loss = move_loss + eval_loss
        
        # Backward pass and optimization
        optimizer.zero_grad()
        total_loss.backward()
        optimizer.step()
        
        print(f"Epoch {epoch}, Loss: {total_loss.item()}")