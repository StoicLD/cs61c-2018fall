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
    #正则匹配查找所有的单词
    #return re.findall(r"\w+", document[1])

    # line ===> (doc id, word)
    return [(document[0], word) for word in re.findall(r"\w+", document[1])]

def mapFunc(arg):
    """ Your code here. """
    #这里对于所有的单词，只是简单地返回来(单词，单词)这种形式，因此结果里就只是这样(word, word)
    #return (arg, 1)

    # (doc_id, word) ===> (word, {doc_id})
    return (arg[1],{ arg[0]})   #become (word, {doc_id} ) 返回的还是一个键值对,其中value是一个只含有一个元素的set

def reduceFunc(arg1, arg2):
    """ Your code here. """
    #return sum(arg1)
    #这里是一个接一个的union，比方说一个单词'the', 是传入的arg1 ('the', {doc1})，而arg2是 ('the', {doc2})
    #因为之前调用的是reduceByKey函数，按照键值相等的元素一个个进行操作，这个操作就是返回 {doc1, doc2},这样一个接着
    #一个的，最后返回{doc1, doc2, doc3,..... }这种形式，于是最终
    #这里的union是python set的内置函数，合并两个set，没有重复的元素，这么做就不需要distinct()函数了。

    #union the set {doc_id1} + {doc_id2}  ===> {doc_id1, doc_id2}
    return arg1.union(arg2)

def lens(arg):

    #(word, {doc_id1, doc_id2, doc_id3...}) ===> (word, length)
    return ( arg[0],len(arg[0]))

def docwordcount(file_name, output="spark-wc-out-docwordcount"):
    sc = SparkContext("local[8]", "DocWordCount")
    file = sc.sequenceFile(file_name)

    #file 是一个序列的文件
    counts = file.flatMap(flatMapFunc) \
                 .map(mapFunc) \
                 .reduceByKey(reduceFunc)\
                 .map(lens)\
                 .sortBykey()
    counts.coalesce(1).saveAsTextFile(output)

""" Do not worry about this """
if __name__ == "__main__":
    argv = sys.argv
    if len(argv) == 2:
        docwordcount(argv[1])
    else:
        docwordcount(argv[1], argv[2])
