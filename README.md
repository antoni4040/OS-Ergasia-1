<h1>OS Project 1</h1>
<h3>by Antonios Karvelas, sdi1600060</h3>
<h4><i>Winter Semester 2019-2020</i></h4>

<h2>Bloom Filter:</h2>
<p>
For the bloom filter I use an array of <strong>uint32_t</strong>, each one holding 32 bits of information. For a given size of input, I triple that and search for the first prime number. I make sure that I have enough <strong>uint32_t</strong>s to hold that many bits (I may end up with a few unused bits, but that's ok).
For the hash function, I used the murmur3 libary found here: <a href="https://github.com/PeterScott/murmur3">murmur3 C implementation</a>.
</p>

<h2>Red-Black Tree:</h2>
<p>
For the red-black tree, I basically use the <i>CLRS</i> implementation of all the algorithms (I hate that book, but it's a good reference). <br>
Each node has a void pointer to its element, a bit for its color(I define 1 as RED and 0 as BLACK for convenience) and pointers to its parent and children.<br>
The RBT struct has function pointers to compare, free and print functions for its nodes. These are necessary for the various RBT operations and need to be different for different types of nodes, so I think this approach is benefitial. It also has a root and NIL node, NIL being the sentinel node.
</p>

<h2>Hashtable:</h2>
<p>
For the hashtable, I define its size and create an array of that size with RBT pointers. These RBTs house postcodeDate, meaning structs with the postcode number and a voters RBT that houses the voters from that specific postcode. It other words, it's a hashtable of RBTs of RBTs. <br> 
This makes operations to have a complexity of O(logn) even if we add way more postcodes than our hashtable can provide constant time access without restructuring it (hashtables are not magic, we can't just keep adding stuff to them and expect O(1) acceess). That never happens with the provided test data, but never mind that.
</p>

<h2>Election Manager Struct:</h2>
<p>
In order to pass all the needed data easily through functions, I create an electionManager struct that houses the bloom filter, the RBT and the hashtable. It's also responsible for the number of updates that happen and executes the bloom filter restructuring when that's necessary.
</p>