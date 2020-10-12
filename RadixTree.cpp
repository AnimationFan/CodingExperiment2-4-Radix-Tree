#include "RadixTree.h"
#include"common.h"
RadixTreeNode::RadixTreeNode() {
	this->str = nullptr;
	this->beginBit = 0;
	this->length = 0;
	this->left = nullptr;
	this->right = nullptr;
	this->itemEnd = false;
}



RadixTreeNode::RadixTreeNode(char* str, int beginBit, int length,bool itemEnd) {
	this->itemEnd = itemEnd;
	this->str = this->cutStr(str, beginBit, length);
	if (this->str == nullptr) {
		/*���������������������ʱ�򷵻�һ���յĽ��*/
		this->beginBit = 0;
		this->length = 0;
		this->left = nullptr;
		this->right = nullptr;
	}
	else {
		this->beginBit = beginBit & 7;
		this->length = length;
		this->left = nullptr;
		this->right = nullptr;
	}
}


/*�ַ�����ȡ*/
/*checked*/
char* RadixTreeNode::cutStr(char* str, int beginBit, int lenBit) {
	int beginChar = beginBit >> 3;
	int endChar = (beginBit + lenBit-1) >> 3;
	int charLen = endChar - beginChar + 1;
	if (endChar < strlen(str) && beginChar < strlen(str)&&lenBit!=0) {
		char* result = new char[ charLen+1];
		result[charLen] = 0x00;
		memcpy(result,&str[beginChar],charLen);
		return result;
	}else
		return nullptr;
}



/*
  ƥ��ʱ�Ӹ�λƥ�䵽��λ
  ���ַ�����beginBit��ʼ��������ַ����Ƚ�
  ������ַ���Ӧ���Ǵ�beginBit& 7��ʼ�Ƚ�
  ���ؽ��Ӧ��Ϊƥ�䲿�ֵĳ���
  �����ǲ���ƥ���������Ӧ������ʹ�õĺ��������ж�
  */
int RadixTreeNode::match(char* str, int beginBit) {
	if (str == nullptr)
		return -1;
	int tarChar = beginBit>>3;
	int srcChar = 0;
	int matchLen = 0;
	int srcBeginBit = this->beginBit;
	for (int i = beginBit;i<(strlen(str)<<3)&& i< beginBit+this->length; i++) {
		unsigned char mask = 128;
		mask = mask >> (i % 8);
		if ((mask & str[tarChar]) == (mask & this->str[srcChar])) {
			matchLen++;
			if (i > 0 && i % 8 == 0 && i!=beginBit) {
				tarChar++;
				srcChar++;
			}
		}
		else break;
	}
	return matchLen;
}

bool RadixTreeNode::adjust(char* str, int beginBit) {
	int matchLen = this->match(str, beginBit);
	if (matchLen <= 0) return false;
	else {
		/*ǡ�õ����������Ҫ���ѽ��*/
		if (this->length == matchLen) {
			return true;
		}
		/*������ѽ������*/

		/*���ݵ�ǰ״�������µ��ӽڵ�*/
		RadixTreeNode* mid = new RadixTreeNode(this->cutStr(this->str, this->beginBit + matchLen, this->length - matchLen),
																			(this->beginBit+matchLen)%8,
																			this->length-matchLen,
																			this->itemEnd);
		
		/*ԭ������*/
		char* preStr = this->str;
		this->str = this->cutStr(this->str, this->beginBit, matchLen);
		delete[] preStr;
		this->length = matchLen;
		this->itemEnd = false;
		int newSize = ((this->beginBit + this->length) >> 3) + (this->length + this->beginBit) % 8 == 0 ? 0 : 1 + 1;
		

		mid->left = this->left;
		mid->right = this->right;
		this->left = mid;
		this->right = nullptr;
	}
	return true;
}




/*���ǵĿ���û�й���ǰ׺������������������һ���յ�ͷ�ڵ�*/
RadixTree::RadixTree() {
	this->root = new RadixTreeNode();
}

RadixTreeNode* RadixTree::getRoot() {
	return this->root;
}

void RadixTreeNode::visist(char* buffer, int curChar){
	/*���õ�ǰ������Ϣ*/
	int m = this->length;

	if (this->beginBit + this->length >= 8) {
		int endChar = curChar + ((this->beginBit + this->length) >> 3);
		for (int i = curChar; i < endChar; i++)
			buffer[i] = this->str[i - curChar];
		buffer[endChar] = '\0';
	}
	/*����������*/
	if (this->left != nullptr) {
		if (this->beginBit + this->length>= 8) {
			int endChar = curChar + ((this->beginBit + this->length) >> 3);
			this->left->visist(buffer, endChar);
		}
		else {
			this->left->visist(buffer, curChar);
		}
			
	}



	if (this->itemEnd == true && (this->beginBit + this->length) % 8 == 0) {
		buffer[curChar + ((this->beginBit + this->length) >> 3)] = '\0';
		printf("%s\n", buffer);
	}


	/*����������*/
	if (this->right != nullptr) {
		if (this->beginBit + this->length >= 8) {
			int endChar = curChar + ((this->beginBit + this->length) >> 3);
			this->right->visist(buffer, endChar);
		}
		else {
			this->right->visist(buffer, curChar);
		}

	}
		

 }


bool RadixTree::put(char* str) {

	RadixTreeNode* node = this->root;
	int beginBit = 0;
	int tolLen = strlen(str)<<3;
	


	while (beginBit< tolLen)
	{
		/*���Ҿ�Ϊ��*/
		if (node->left == nullptr && node->right == nullptr) {
			node->left = new RadixTreeNode(str, beginBit, tolLen - beginBit, true);
			node = node->left;
			beginBit += node->length;
			continue;
		}
		
		int leftMatchLen = -1, rightMatchLen = -1;
		if(node->left!=nullptr)
			leftMatchLen = node->left->match(str, beginBit);
		if(node->right != nullptr)
			rightMatchLen = node->right->match(str, beginBit);
		
		/*����Ϊ������һ����ȫ��ƥ��*/
		if (leftMatchLen == 0 && node->right == nullptr) {
			node->right= new RadixTreeNode(str, beginBit, tolLen - beginBit, true);
			node = node->right;
			beginBit += node->length;
			continue;
		}
		if (rightMatchLen == 0 && node->left == nullptr) {
			node->left = new RadixTreeNode(str, beginBit, tolLen - beginBit, true);
			node = node->left;
			beginBit += node->length;
			continue;
		}

		/*���ٴ���һ��ƥ��*/ 
		if (leftMatchLen >= 1) {
			node = node->left;
			node->adjust(str,beginBit);
			beginBit += node->length;
			continue;
		}
		if (rightMatchLen >= 1) {
			node = node->right;
			node->adjust(str,beginBit);
			beginBit += node->length;
			continue;
		}

	}

	if (tolLen == beginBit) {
		node ->itemEnd = true;
		return true;
	}
	return false;
}

bool RadixTree::get(char* str) {
	RadixTreeNode* node = this->root;
	int beginBit = 0;
	int totalLen = strlen(str) << 3;
	while (beginBit < totalLen) {
		int leftMatchLen = -1,rightMatchLen =-1;
		if (node->left != nullptr)
			leftMatchLen = node->left->match(str, beginBit);
		if (node->right != nullptr)
			rightMatchLen = node->right->match(str, beginBit);
		if (leftMatchLen <= 0 && rightMatchLen <= 0)
			break;
		if (leftMatchLen > rightMatchLen&& leftMatchLen == node->left->length) {
			node = node->left;
			beginBit += leftMatchLen;
			continue;
		}
		if (leftMatchLen < rightMatchLen && rightMatchLen == node->right->length) {
			node = node->right;
			beginBit += rightMatchLen;
			continue;
		}
		break;
	}
	if (totalLen == beginBit && node->itemEnd == true)
		return true;
	else
		return false;
	return false;
}