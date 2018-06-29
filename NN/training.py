import numpy as np
import tensorflow as tf
from tensorflow.python.platform import gfile
import csv
import collections

num_fan_classes = 2
num_pump_classes = 2

Dataset = collections.namedtuple('Dataset', ['data', 'target'])

def load_csv_without_header(fn, target_dtype, features_dtype, target_columns):
    with gfile.Open(fn) as csv_file:
        data_file = csv.reader(csv_file)
        data = []
        targets = {
            target_cols: []
            for target_cols in target_columns.keys()
        }
        for row in data_file:
            cols = sorted(target_columns.items(), key=lambda tup: tup[1], reverse=True)
            for target_col_name, target_col_i in cols:
                targets[target_col_name].append(row.pop(target_col_i))
            data.append(np.asarray(row, dtype=features_dtype))

        targets = {
            target_col_name: np.array(val, dtype=target_dtype)
            for target_col_name, val in targets.items()
        }
        data = np.array(data)
        return Dataset(data=data, target=targets)

feature_columns = [
    tf.contrib.layers.real_valued_column("", dimension=1),
    tf.contrib.layers.real_valued_column("", dimension=2),
]
head = tf.contrib.learn.multi_head([
    tf.contrib.learn.multi_class_head(
        num_fan_classes, label_name="fan", head_name="fan"),
    tf.contrib.learn.multi_class_head(
        num_pump_classes, label_name="pump", head_name="pump"),
])
classifier = tf.contrib.learn.DNNEstimator(
    feature_columns=feature_columns,
    hidden_units=[10, 20, 10],
    model_dir="iris_model",
    head=head,
)

def get_input_fn(filename):
    def input_fn():
        dataset = load_csv_without_header(
            fn=filename,
            target_dtype=np.int,
            features_dtype=np.int,
            target_columns={"fan": 4, "pump": 5}
        )
        x = tf.constant(dataset.data)
        y = {k: tf.constant(v) for k, v in dataset.target.items()}
        return x, y
    return input_fn

classifier.fit(input_fn=get_input_fn("Dataset/train_dataset.csv"), steps=4000)
res = classifier.evaluate(input_fn=get_input_fn("Dataset/test_dataset.csv"), steps=1)

print("Validation:", res)
