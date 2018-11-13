import sys
import os
import tensorflow as tf

os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'
tf.logging.set_verbosity(tf.logging.ERROR) #avoid print warnings

model_path = "/home/himasha/Desktop/FYP/EMCS-greenhouse/NN/Model"

def multilayer_perceptron(x, weights, biases):
    layer_1 = tf.add(tf.matmul(x, weights['h1']), biases['b1'])
    layer_1 = tf.nn.relu(layer_1)

    layer_2 = tf.add(tf.matmul(layer_1, weights['h2']), biases['b2'])
    layer_2 = tf.nn.relu(layer_2)

    out_layer = tf.add(tf.matmul(layer_2, weights['out']), biases['out'],name="op_to_restore")
    return out_layer

def predictint(values):
    n_hidden_1 = 10
    n_hidden_2 = 20
    n_input = 4
    n_classes = 4

    x = tf.placeholder(tf.float32, shape=[None, n_input], name="x")  # 26.7,927,97.8,34
    # y = tf.placeholder(tf.float32, shape=[None, n_classes], name="y")

    weights = {
        'h1': tf.Variable(tf.zeros([n_input, n_hidden_1]), name="h1"),
        'h2': tf.Variable(tf.zeros([n_hidden_1, n_hidden_2]), name="h2"),
        'out': tf.Variable(tf.zeros([n_hidden_2, n_classes]), name="out_w")
    }

    biases = {
        'b1': tf.Variable(tf.zeros([n_hidden_1]), name="b1"),
        'b2': tf.Variable(tf.zeros([n_hidden_2]), name="b2"),
        'out': tf.Variable(tf.zeros([n_classes]), name="out_b")
    }

    pred = multilayer_perceptron(x, weights, biases)

    cross_entropy = tf.nn.softmax(logits=pred)

    init_op = tf.global_variables_initializer()
    saver = tf.train.Saver()

    with tf.Session() as sess:
        sess.run(init_op)
        saver.restore(sess, tf.train.latest_checkpoint(model_path))

        prediction = tf.argmax(cross_entropy, 1)
        return prediction.eval(feed_dict={x: [values]}, session=sess)

def main(argv1,argv2,argv3,argv4):
    predint = predictint([argv1,argv2,argv3,argv4])
    print (predint[0])

if __name__ == "__main__":
    main(sys.argv[1],sys.argv[2],sys.argv[3],sys.argv[4])