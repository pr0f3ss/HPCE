import torch
import torch.nn as nn
import torch.nn.functional as F

class ChessTransformer(nn.Module):
    def __init__(self, token_dim=112, embed_dim=256, num_heads=8, num_layers=6, num_tokens=64):
        super(ChessTransformer, self).__init__()
        
        # Token embedding
        self.token_embedding = nn.Linear(token_dim, embed_dim)
        
        # Learned offset vectors
        self.offset_add = nn.Parameter(torch.randn(num_tokens, embed_dim))
        self.offset_mul = nn.Parameter(torch.randn(num_tokens, embed_dim))
        
        # Transformer encoder
        encoder_layer = nn.TransformerEncoderLayer(d_model=embed_dim, nhead=num_heads)
        self.transformer_encoder = nn.TransformerEncoder(encoder_layer, num_layers=num_layers)
        
        # Output heads (example: move prediction and evaluation)
        self.move_head = nn.Linear(embed_dim, 64 * 64)  # 64x64 move probabilities
        self.eval_head = nn.Linear(embed_dim, 1)        # Position evaluation

    def forward(self, x):
        # x: (batch_size, num_tokens, token_dim)
        
        # Token embedding
        x = self.token_embedding(x)  # (batch_size, num_tokens, embed_dim)
        
        # Add and multiply by learned offset vectors
        x = (x + self.offset_add) * self.offset_mul
        
        # Transformer encoder
        x = x.permute(1, 0, 2)  # (num_tokens, batch_size, embed_dim)
        x = self.transformer_encoder(x)
        x = x.permute(1, 0, 2)  # (batch_size, num_tokens, embed_dim)
        
        # Output heads
        move_logits = self.move_head(x)  # (batch_size, num_tokens, 64 * 64)
        eval_score = self.eval_head(x.mean(dim=1))  # (batch_size, 1)
        
        return move_logits, eval_score

# Example usage
if __name__ == "__main__":
    # Hyperparams
    batch_size = 32
    token_dim = 112
    num_tokens = 64

    # TODO: fill input tensor with training data
    input_tensor = torch.randn(batch_size, num_tokens, token_dim)

    # Initialize the model
    model = ChessTransformer(token_dim=token_dim)

    # Forward pass
    move_logits, eval_score = model(input_tensor)

    print("Move logits shape:", move_logits.shape)  # Expected: (batch_size, num_tokens, 64 * 64)
    print("Evaluation score shape:", eval_score.shape)  # Expected: (batch_size, 1)