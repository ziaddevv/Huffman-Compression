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
        ch='\n';
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
  void init()
  {
    for(auto &[ch,f]:freq)
    {
        
        TreeNode*newNode=new TreeNode(f,ch);
        pq.push(newNode);
    }
    //update rely on the bool isLeaf rather than # char
    // while(!pq.empty())
    // {
    //   cout<<pq.top()->ch<<" "<<pq.top()->val<<"\n";
    //   pq.pop();
    // }
  }
  void BuildHofmanTree()
  {
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
  void dfs(TreeNode*p)
  {
    if(p==nullptr)return;
    
    cout<<p->ch<<" "<<p->val<<"\n";
    dfs(p->left);
    dfs(p->right);
  }
  void dispaly()
  {
    dfs(root);
  }
  void Traverse(TreeNode*p,string cur)
  {
    if(p==nullptr)return;
    //we update here to check the isLeaf also
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
    for(auto it:encoded)
    {
      cout<<it.first<<" "<<it.second<<"\n";
    }

  }
int decodeChar(TreeNode* p, int idx, const string& line, string& res) {
    while (p->left || p->right) {  
        if (line[idx] == '0')
            p = p->left;
        else
            p = p->right;
        idx++;
    }
    //once left and right are null
    res.push_back(p->ch);  
    return idx;
}
//010101

string getLine(const string& line) {
    string res = "";
    int idx = 0;
    while (idx < (int)line.size()) {
        idx = decodeChar(root, idx, line, res);
    }
    return res;
}
 

};