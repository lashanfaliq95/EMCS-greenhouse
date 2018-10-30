import tensorflow as tf
import matplotlib.pyplot as plt
import pandas as pd

tf.reset_default_graph()   # To clear the defined variables and operations of the previous cell

df = pd.read_csv('Dataset/new_dataset.csv')
print(df.shape)
input_data = df.values[:, 4:8]
labels = pd.get_dummies(df['9'])

train_input = input_data[:2000]
test_input = input_data[2000:]

train_labels = labels[:2000]
test_labels = labels[2000:]

print(train_labels.shape[0])

learning_rate = 0.2
training_epochs = 50
batch_size = 100
display_step = 1

n_hidden_1 = 10
n_hidden_2 = 20
n_input = 4
n_classes = 4

model_path = "/home/himasha/Desktop/FYP/EMCS-greenhouse/NN/Model/model.ckpt"

x = tf.placeholder(tf.float32, shape=[None, n_input],name="x")   #26.7,927,97.8,34
y = tf.placeholder(tf.float32, shape=[None, n_classes],name="y")


def multilayer_perceptron(x, weights, biases):
    layer_1 = tf.add(tf.matmul(x, weights['h1']), biases['b1'])
    layer_1 = tf.nn.relu(layer_1)

    layer_2 = tf.add(tf.matmul(layer_1, weights['h2']), biases['b2'])
    layer_2 = tf.nn.relu(layer_2)

    out_layer = tf.add(tf.matmul(layer_2, weights['out']), biases['out'],name="op_to_restore")
    return out_layer


weights = {
    'h1': tf.Variable(tf.random_normal([n_input, n_hidden_1]), name="h1"),
    'h2': tf.Variable(tf.random_normal([n_hidden_1, n_hidden_2]), name="h2"),
    'out': tf.Variable(tf.random_normal([n_hidden_2, n_classes]), name="out_w")
}

biases = {
    'b1': tf.Variable(tf.random_normal([n_hidden_1]), name="b1"),
    'b2': tf.Variable(tf.random_normal([n_hidden_2]),name="b2"),
    'out': tf.Variable(tf.random_normal([n_classes]),name="out_b")
}

pred = multilayer_perceptron(x, weights, biases)

cross_entropy = tf.nn.softmax_cross_entropy_with_logits(logits=pred, labels=y)
cost = tf.reduce_mean(cross_entropy)
optimizer = tf.train.AdamOptimizer(learning_rate=learning_rate).minimize(cost)

init = tf.global_variables_initializer()
saver = tf.train.Saver()

cost_history = []
accuracy_history = []

with tf.Session() as sess:
    sess.run(init)

    for epoch in range(training_epochs):
        avg_cost = 0,
        total_batch = int(train_labels.shape[0] / batch_size)

        for i in range(total_batch):
            #batch_x, batch_y = mnist.train.next_batch(batch_size)
            batch_x = train_input[i*batch_size:(i+1)*batch_size]
            batch_y = train_labels[i * batch_size:(i + 1) * batch_size]
            _, c = sess.run([optimizer, cost], feed_dict={x: batch_x, y: batch_y})

            avg_cost += c / total_batch

        if epoch % display_step == 0:

            correct_prediction = tf.equal(tf.argmax(pred, 1), tf.argmax(y,1))

            accuracy = tf.reduce_mean(tf.cast(correct_prediction,"float"))
            acu_temp = accuracy.eval({x: test_input, y: test_labels})


            accuracy_history.append(acu_temp)

            cost_history.append(avg_cost)
            print("epoch: ",'%04d' % (epoch+1), "cost=", '%05f '%(avg_cost), " Accuracy=", '%05f '%(acu_temp*100))

    print("Optimization finished")
    writer = tf.summary.FileWriter('Graphs', sess.graph)
    save_path = saver.save(sess, model_path)
    print("Model saved in file %s" % save_path)

    plt.plot(cost_history)
    plt.show()
    plt.plot(accuracy_history)
    plt.show()

    correct_prediction = tf.equal(tf.argmax(pred, 1), tf.argmax(y,1))

    accuracy = tf.reduce_mean(tf.cast(correct_prediction, "float"))
    print("Accuracy : ", accuracy.eval({x: test_input, y: test_labels}))

