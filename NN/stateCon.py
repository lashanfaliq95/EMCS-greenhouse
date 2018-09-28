import pandas as pd

df = pd.read_csv("Dataset/train_dataset.csv", header=None)
df = df.dropna()
# print df[df.columns[7:9]]
df['state'] = 0
# print( df)
# print df.values[0][7:9]
matrix = df.values
shape = matrix.shape
for x in range(shape[0]):
    targets = matrix[x][7:9]
    if (targets[0]==0 and targets[1]==0):
        matrix[x][9] = 0

    elif (targets[0]==0 and targets[1]==1):
        matrix[x][9] = 1

    elif (targets[0]==1 and targets[1]==0):
        matrix[x][9] = 2

    elif (targets[0]==1 and targets[1]==1):
        matrix[x][9] = 3
df = df.dropna()
# df[[7,8,'state']] = df[[7, 8,'state']].astype(int)
# print(df)
new_df = pd.DataFrame(matrix)
print(new_df)
new_df.to_csv('Dataset/new_dataset.csv')
