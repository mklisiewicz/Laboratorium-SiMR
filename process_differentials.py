import pandas as pd

# Read the CSV file
df = pd.read_csv('differentials.csv', sep=';')

# Define the mapping
replacements = {
    'cos\(gamma\)': "G",
    'sin\(gamma\)': 'g',
    'cos\(beta\)': "b",
    'sin\(beta\)': 'B',
    'sin\(alpha\)': 'A',
    'cos\(alpha\)': "a"
}

# Apply the replacements to the second column
df.iloc[:, 1] = df.iloc[:, 1].replace(replacements, regex=True)

df.to_csv('processed_differentials.csv', sep=';', index=False)