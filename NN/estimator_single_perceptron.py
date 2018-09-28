# single layer perceptron
import tensorflow as tf
import pandas as pd

df = pd.read_csv('Dataset/new_dataset.csv')
print(df.shape)
input_data = df.values[:, 4:8]
labels = pd.get_dummies(df['9'])

train_input = input_data[:2000]
test_input = input_data[2000:]

train_labels = labels[:2000]
test_labels = labels[2000:]


sess = tf.InteractiveSession()

x = tf.placeholder(tf.float32, shape=[None, 4])
y = tf.placeholder(tf.float32, shape=[None, 4])

w = tf.Variable(tf.zeros([4, 4]))
b = tf.Variable(tf.zeros([4]))

sess.run(tf.global_variables_initializer())

y_ = tf.matmul(x, w) + b

cross_entropy = tf.reduce_mean(
    tf.nn.softmax_cross_entropy_with_logits(labels=y, logits=y_)
)

train_step = tf.train.GradientDescentOptimizer(0.5).minimize(cross_entropy)


train_step.run(feed_dict={x: train_input, y: train_labels})

correct_prediction = tf.equal(tf.argmax(y_, 1), tf.argmax(y, 1))

accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

print(accuracy.eval(feed_dict={x: test_labels, y: test_labels})*100)
