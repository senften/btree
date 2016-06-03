/*
 * C++ Program to Implement B-Tree
 */

// full == 2t-1 where t is the degree (default 3)
//

#include <stdio.h>
#include <iostream>
#include <algorithm> // find
#include <cstdlib> // atoi
#include <queue> // std::queue
#include <random>
#include <memory> // shared_ptr

using namespace std;

class BTreeNode;
typedef std::shared_ptr<BTreeNode> BTreeNode_t;

void bubblesort(int *p, int n);

class BTreeNode
{
public:
    BTreeNode();
    BTreeNode( unsigned int degree );
    bool full();
    bool leaf() {return _leaf;}
    bool addChild(unsigned int loc, BTreeNode_t child);
    BTreeNode_t getChild( unsigned int loc );
    bool addKey( int key );
// protected:
    int *_data; // array of data
    std::vector<BTreeNode_t> _child_ptr; // array of children
    // std::vector< std::shared_ptr<BTreeNode> > _child_ptr; // array of children
    bool _leaf;
    int _n; // num indicies, if not leaf num children = n+1
    const unsigned int _degree;
private:
    void init();
};

BTreeNode::BTreeNode() : _degree( 3 )
{
    init();
}

BTreeNode::BTreeNode( unsigned int degree ) : _degree( degree ) {
    init();
}

void BTreeNode::init()
{
    _data = new int[5]; // _degree*2-1
//    _child_ptr = new BTreeNode *[6]; // _degree*2
    _leaf = true;

    _n = 0;

    // for (int i = 0; i < 6; i++) {
    //     _child_ptr[i] = NULL;
    // }
}

bool BTreeNode::full()
{
    if ( _n==5 ) return true;
    return false;
}

bool BTreeNode::addChild( unsigned int loc, BTreeNode_t child )
{
    auto it = _child_ptr.begin();
    it+=loc;
    _child_ptr.insert( it, child );
    _leaf = false;
    return true;
}

BTreeNode_t BTreeNode::getChild( unsigned int loc )
{
    auto it = _child_ptr.begin();
    it+=loc;
    return *it;
}

bool BTreeNode::addKey( int key )
{
    _data[_n] = key;
    bubblesort(_data, _n);
    _n++;
}

class BTree
{
public:
    BTree();
    BTreeNode_t root() { return _root;} // should be a smart pointer

    void insertKey( int key );
    bool insertNonfull (BTreeNode_t node, int key);

protected:
    BTreeNode_t _root;
    const unsigned int _degree;
};
typedef std::shared_ptr<BTree> SharedBTree_t;
typedef std::shared_ptr<BTree> BTree_t;

BTree::BTree() : _degree( 3 )
{
    // _root=init();
    _root = BTreeNode_t(new BTreeNode());
}

void traverse(BTreeNode_t p)
{

    cout<<endl;

    int i;
    for (i = 0; i < p->_n; i++) {

        if (!p->leaf()) {
            traverse(p->_child_ptr[i]);
        }

        cout << " " << p->_data[i];
    }

    // Any node has n+1 children
    if (p->leaf() == false) {
        traverse(p->_child_ptr[i]);
    }

    cout<<endl;

}

void printTree(BTreeNode_t p)
{

    cout<<endl;

    std::queue<BTreeNode_t> levels;
    levels.push( p );

    // traverse all kids

    while ( !levels.empty() ) {

        // pop head
        BTreeNode_t node = levels.front(); levels.pop();
        if ( node ) {
            // add children
            if (!node->leaf()) {
                for (int i = 0; i<=node->_n; i++) {    // Any node has n+1 children
                    levels.push(node->_child_ptr[i]);
                }
                levels.push( nullptr );
            }
            // process popped node

            for (int i = 0; i < node->_n; i++) {
                cout << " " << node->_data[i];
            }
            std::cout << " | ";
        }
        else {
            std::cout << std::endl;
        }

    }

    std::cout << std::endl;
}

void bubblesort(int *p, int n)

{

    int i, j, temp;

    for (int i = 0; i < n; i++) {
        for (int j = i; j <= n; j++) {

            if (p[i] > p[j]) { // simple bubble sort

                temp = p[i];
                p[i] = p[j];
                p[j] = temp;

            }
        }
    }
}

int split_child(BTreeNode_t parent, unsigned int loc, BTreeNode_t y)
{
    // need a new node
    auto newNode = BTreeNode_t(new BTreeNode());
    newNode->_leaf = y->leaf(); // if y is a leaf so is the newNode
    // copy right half of y into newNode
    // t to 2t-1
    for (int j = 3; j < 5; j++) { // move backend of members [3,4] to new node

        newNode->_data[j - 3] = y->_data[j];
        newNode->_n++;

        y->_data[j] = 0; // 0 out end
        y->_n--;
    }

    // copy over child pointers if y is not a leaf
    if ( !y->leaf() ) {
        int j;
        auto it = y->_child_ptr.begin();
//        for ( j=0, it+=_degree; it!=y->_child_ptr.end(); j++, it++ ) {
        for ( j=0, it+=3; it!=y->_child_ptr.end(); j++, it++ ) {
            newNode->addChild( j, *it );
        }
    }

    // insert new child newNode in parent *after* y
    parent->addChild( loc+1, newNode );
    // insert key
    // ...to accomodate the new key we're bringing in from the middle
    // of y (if you're wondering, since (t-1) + (t-1) = 2t-2, where
    // the other key went, its coming into x)

    // int mid = y->_data[_degree-1];
    int mid = y->_data[2];
    y->_data[2] = 0; y->_n--; // moving mid point

    parent->addKey( mid );

}

bool BTree::insertNonfull (BTreeNode_t node, int key)
{
    // int i = node->_n;

    if ( node->leaf() ) {
        node->addKey( key );
    }
    else {
        // find child where new key belongs:

        bool found = false;
        int i = 0;
        for ( i=node->_n-1, found=false; i>=0; i-- ) {
            if ( key > node->_data[i] ) {
                found = true;
                break;
            }
            // while i >= 1 and k < keyi[x] do
            // 	i--
            // end while
        }
        // i+1 is my desired child

        // if k is in ci[x], then k <= keyi[x] (from the definition)
        // we'll go back to the last key (least i) where we found this
        // to be true, then read in that child node

//        if ( found ) i++;
        i++;
//		Disk-Read (ci[x])
        auto child = node->getChild( i );
        if ( child->full() ) {
            // uh-oh, this child node is full, we'll have to split it

            split_child (node, i, child); //

            // now ci[x] and ci+1[x] are the new children,
            // and keyi[x] may have been changed.
            // we'll see if k belongs in the first or the second

            if ( key > node->_data[i] /*keyi[x]*/ ) {
                child = node->getChild( ++i );
            }
        }

        // call ourself recursively to do the insertion
        insertNonfull(child, key);
        // B-Tree-Insert-Nonfull (ci[x], k)
    }
}

void BTree::insertKey(int a)
{
    if ( root()->full() ) { // root is full

        // uh-oh, the root is full, we have to split it
		// s = allocate-node ()
		// root[T] = s 	// new root node
		// leaf[s] = False // will have some children
		// n[s] = 0	// for now
		// c1[s] = r // child is the old root node
		// B-Tree-Split-Child (s, 1, r) // r is split
		// B-Tree-Insert-Nonfull (s, k) // s is clearly not full

        BTreeNode_t oldRoot = root();
        BTreeNode_t newRoot = BTreeNode_t(new BTreeNode());

        _root = newRoot;
        newRoot->addChild( 0, oldRoot ); // child is the old root node; gonna split it

        split_child( newRoot, 0, oldRoot );
        insertNonfull( newRoot, a ); // newRoot is not full, clearly
    }
    else {
        insertNonfull( root(), a );
    }
}


char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

int main(int argc, char *argv[])

{

    int n( 50 );
    auto tree =  BTree_t(new BTree());;

    if (cmdOptionExists(argv, argv+argc, "--count"))
    {
        n = atoi( getCmdOption(argv, argv+argc, "--count") ); // should probably use boost lexical cast
    }

    // Seed with a real random value, if available
    std::random_device r;

    // Choose a random mean between 1 and 6
//    std::default_random_engine e1(r());
    std::default_random_engine e1(42);
    std::uniform_int_distribution<int> uniform_dist(0, 1000); // c++-11 uniform random number

    for(int i = 0; i < n; i++) {
          tree->insertKey(uniform_dist( e1 ));
          printTree( tree->root() );
          std::cout << "-----" << std::endl;
    }

    cout<<"traversal of constructed tree\n";

    traverse(tree->root());

//    getch();

}
