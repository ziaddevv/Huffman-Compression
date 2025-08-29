#define ll long long 
#include <iostream>
#include <vector>
#include <algorithm>
#include<unordered_map>
#include <map>
#include <queue>
#include <climits>
#include<set>
#include<fstream>
using namespace std;

struct TreeNode
{
    TreeNode(int v)
    {
        val=v;
        ch='\0';
        isLeaf = false;  
        left = right = nullptr;
    }
    TreeNode(int v,char c)
    {
        val=v;
        ch=c;
        isLeaf=true;
        left=nullptr;
        right=nullptr;
    }
    long long val;
    char ch;
    bool isLeaf;
    TreeNode*left;
    TreeNode*right;
};

class Compare {
public:
	bool operator()(TreeNode* a, TreeNode* b) {
		if(a->val > b->val) {
			return true;
		}
		return false;
	}
};
class Tree
{
  public:
  TreeNode*root;
  
  unordered_map<char,int>freq;
  unordered_map<char,string>encoded;
  priority_queue<TreeNode*,vector<TreeNode*>,Compare>pq;
  Tree(unordered_map<char,int>f)
  {
    freq=f;
  }
  Tree(){
    root=nullptr;
  }
  void init()
  {
    for(auto &[ch,f]:freq)
    {
        
        TreeNode*newNode=new TreeNode(f,ch);
        pq.push(newNode);
    }
    //update rely on the boolean isLeaf rather than # char

    // while(!pq.empty())
    // {
    //   cout<<pq.top()->ch<<" "<<pq.top()->val<<"\n";
    //   pq.pop();
    // }
  }
void rebuildTree(const unordered_map<char,string>& encoded) {
    root = new TreeNode(0);  // root with no char
    root->isLeaf = false;

    for (auto& [ch, code] : encoded) {
        TreeNode* cur = root;
        for (char bit : code) {
            if (bit == '0') {
                if (!cur->left) {
                    cur->left = new TreeNode(0);
                    cur->left->isLeaf = false;
                }
                cur = cur->left;
            } else {
                if (!cur->right) {
                    cur->right = new TreeNode(0);
                    cur->right->isLeaf = false;
                }
                cur = cur->right;
            }
        }
        cur->ch = ch;
        cur->isLeaf = true;
    }
}

  void BuildHofmanTree()
  {
    if (pq.size() == 1) {
        root = pq.top(); 
        pq.pop();
        return;
    }
    while(!pq.empty())
    {
      TreeNode*left=pq.top();
      pq.pop();
      TreeNode*right=pq.top();
      pq.pop();
      root=new TreeNode(left->val+right->val);
      //send value only
      root->left=left;
      root->right=right;
      if(pq.empty())
      {
        break;
      }
      pq.push(root);
    }
  }
  void Traverse(TreeNode*p,string cur)
  {
    if(p==nullptr)return;
    //we update here to check the isLeaf also no char #
    if(p->isLeaf==true)
    {
      encoded[p->ch]=cur;
      return;
    }
    Traverse(p->left,cur+'0');
    Traverse(p->right,cur+'1');
    
  }
  void encode()
  {
    Traverse(root,"");


  }
int decodeChar(TreeNode* p, int idx, const string& line, string& res) {
    while (!p->isLeaf) {  // keep movving until we hit a leaf
        if (line[idx] == '0')
            p = p->left;
        else
            p = p->right;
        idx++;
    }
    res.push_back(p->ch);  
    return idx;
}

string DecodeStream(const string& line) {
    string res;
    int idx = 0;
    while (idx < (int)line.size()) {
        idx = decodeChar(root, idx, line, res);
    }
    return res;
}

 

};