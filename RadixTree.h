#pragma once

class RadixTreeNode {
public:
	/*当前结点对应的字符串*/
	char* str;
	/*当前字符串对应的起始位*/
	int beginBit;
	/*结点中保存的对应的数据位的长度*/
	int length;
	/*表示当前结点是否为以字符串的结尾*/
	bool itemEnd;
	/*左子树与右子树*/
	RadixTreeNode* left, *right;
	
	/*构造函数，构造空的结点*/
	RadixTreeNode();
	/*构造函数，可以直接设定几个主要的参数*/
	RadixTreeNode(char* str, int beginBit, int length, bool itemEnd);
	/*匹配函数，将输入的字符串从起始位开始与本身存储的字符串的起始位比较
		并返回最后匹配的长度
	*/
	int match(char* str, int beginBit);
	/*结点调整函数，功能是分裂结点，将
		根据输入的字符串将结点分裂为与字符串完全匹配的和不匹配的两个部分，
		不配的结点将会接在匹配的结点后方
	*/
	bool adjust(char* str, int beginBit);

	/*截取字符串函数*/
	static char* cutStr(char* str, int beginBit, int lenBit);
	/*遍历以当前结点为根的RadxiTree*/
	void visist(char* buffer,  int curChar);
};

class RadixTree
{
private:
	/*根节点*/
	RadixTreeNode* root;
public:
	/*构造函数*/
	RadixTree();
	RadixTreeNode* getRoot();
	/*存字符串*/
	bool put(char* str);
	/*取字符串*/
	bool get(char* str);
};

