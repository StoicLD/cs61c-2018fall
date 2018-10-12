import sys
import re

from pyspark import SparkContext

def flatMapFunc(document):
    """
    Takes in document, which is a key, value pair, where document[0] is the
    document ID and document[1] is the contents of the document. We want
    a distinct list of all the words in each document to pass in to our map
    function, but if we call the transformation distinct() on all the words from
    all the documents with the current code, we won't get quite what we want. How
    can we use the document ID to help get around that problem?
    """
    """ Your code here. """
    
    return [(document[0],word) for word in re.findall(r"\w+", document[1])]

def mapFunc(arg):
    """ Your code here. """
   # return (arg,1)
    return (arg[1],{ arg[0]})   #become (word, doc_id)

def reduceFunc(arg1, arg2):
    """ Your code here. """
   #return 
    return arg1.union(arg2)
def lens(arg):
	return (arg[0],len(arg[1]))
def docwordcount(file_name, output="spark-wc-out-docwordcount"):
    sc = SparkContext("local[8]", "DocWordCount")
    file = sc.sequenceFile(file_name)
    #最后sort by key按照key的字典序排序
    counts = file.flatMap(flatMapFunc) \
                 .map(mapFunc) \
                 .reduceByKey(reduceFunc)\
		 .map(lens)\
		.sortByKey()
    counts.coalesce(1).saveAsTextFile(output)

""" Do not worry about this """
if __name__ == "__main__":
    argv = sys.argv
    if len(argv) == 2:
        docwordcount(argv[1])
    else:
        docwordcount(argv[1], argv[2])
