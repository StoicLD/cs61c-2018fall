import sys
import re

from pyspark import SparkContext

def flatMapFunc(document):
    """
    Takes in document, which is a key, value pair, where document[0] is the
    document ID and document[1] is the contents of the document.
    HINT: You need to keep track of three things, word, document ID, and the
    index inside of the document, but you are working with key, value pairs.
    Is there a way to combine these three things and make a key, value pair?
    """
    """ Your code here. """
    #说明：这里讲word和文档号组成key，然后index单独在

    # ((word1, doc_id1), [index1]), (word2, doc_id1), [index2]), .... )
    return ( ((word, document[0]), [index])\
            for index,word in enumerate(re.findall(r"\w+", document[1])))

def mapFunc(arg):
    """ Your code here. """
    #()
    return (arg, arg)

def reduceFunc(arg1, arg2):
    """ Your code here. """
    return arg1 + arg2

def index(file_name, output="spark-wc-out-index"):
    sc = SparkContext("local[8]", "Index")
    file = sc.sequenceFile(file_name)

    indices = file.flatMap(flatMapFunc) \
                  .reduceByKey(reduceFunc)

    indices.coalesce(1).saveAsTextFile(output)

""" Do not worry about this """
if __name__ == "__main__":
    argv = sys.argv
    if len(argv) == 2:
        index(argv[1])
    else:
        index(argv[1], argv[2])
