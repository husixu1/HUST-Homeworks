#pragma once

#include "config.h"
#include <iostream>
#include <string>
#include <vector>

namespace LCRSTreeConst{
	typedef enum{
		no_error,
		error_node_empty,
		error_opening_file,
		error_file_corrupt,
		error_wrong_file_type
	}error_t;
}

template <typename ElementType>
class LCRSTree{
	
private:
	typedef struct _Node{
		ElementType *element;
		struct _Node *child;
		struct _Node *parent;
		struct _Node *rSibling;
		struct _Node *lSibling;
		
		_Node();
		_Node(const ElementType &);
		~_Node();
		
		_Node &operator=(_Node &);
	}Node;
	
	Node *root;

protected:
	bool doesTrowException;
	inline void throwException(LCRSTreeConst::error_t);
	
public:
	typedef class _iterator{
	private:
		Node *current;
		bool doesTrowException;
		
		// throw exception if does throw exception is true
		inline void throwException(LCRSTreeConst::error_t);
		_iterator(Node *, bool doesThrowException=true);
	public:
		friend class LCRSTree;
		_iterator(bool doesThrowException = true);
		~_iterator();

		// return null if value is empty
		ElementType *getValue();
		void assignData(const ElementType &);
 		
		_iterator &operator++();
		_iterator &operator++(int);
		bool operator==(const _iterator &);
		bool operator!=(const _iterator &);
		_iterator &operator=(const _iterator &);
		ElementType *operator*();
		bool isEmpty(void);

		_iterator rSibling(void);
		_iterator lSibling(void);
		_iterator firstChild(void);
		_iterator parent(void);

		void insertChildAtFront(const ElementType &);
		void insertChildAtTail(const ElementType &);
		
		void insertLSibling(const ElementType &);
		void insertRSibling(const ElementType &);

		void insertSiblingAtFront(const ElementType &);
		void insertSiblingAtTail(const ElementType &);

	}iterator;

	iterator begin() const;
	// when iterating, iterator(nullptr) stands for the end
	iterator end() const;
	
	////------------------------------------------------------
	LCRSTree();
	LCRSTree(bool doesThrowException);
	~LCRSTree();
	
	bool isEmpty();
	
	void insertChildAtFront(iterator &, const ElementType &);
	void insertChildAtTail(iterator &, const ElementType &);

	void insertRSibling(iterator &, const ElementType &);
	void insertLSibling(iterator &, const ElementType &);
	
	void insertSiblingAtFront(iterator &, const ElementType &);
	void insertSiblingAtTail(iterator &, const ElementType &);

	ElementType *getRoot();
	void assignRoot(const ElementType &);
	
	void traverse(void (*visit)(ElementType &));
	void traverse(void(*visit)(int level, ElementType &, void* user_data), void *user_data = nullptr);

	ElementType *findFirst(const ElementType &, bool(*compare)(const ElementType &, const ElementType &));
	// return a match list
	std::vector<ElementType *> find(const ElementType &, bool(*compare)(const ElementType &, const ElementType &));
	// find in the finding result
	std::vector<ElementType *> &find(std::vector<ElementType *> &, const ElementType &, bool(*compare)(const ElementType &, const ElementType &));
	
	LCRSTreeConst::error_t checkIsFileValid(std::string);
	void saveTo(std::string , void (*write_an_elem)(std::ofstream &, ElementType &));
	void loadFrom(std::string , void (*load_an_elem)(std::ifstream &, ElementType &));
	
	void destroy();

#ifdef STATUS_TEST
	void show_HTML_Tree(void);
#endif
};

#ifdef STATUS_TEST
	
#include "DOM_tree_ADT.h"
#include <iomanip>

	template <typename ElementType>
	void LCRSTree<ElementType>::show_HTML_Tree() {
		using namespace std;
		using namespace LCRSTreeConst;
		Node *tempNode = root;
		unsigned int depth = 0;
		for(;tempNode;) {
			if (tempNode->child) {
				tempNode = tempNode->child;
				depth++;
			}
			else if (tempNode->rSibling)
				tempNode = tempNode->rSibling;
			else {
				for (; tempNode && !tempNode->rSibling; tempNode = tempNode->parent, depth--);
				// if has rSibling
				if (tempNode)
					tempNode = tempNode->rSibling;
				else
					break;
			}
			cout << setw(depth * 4) << " " << left << '<' << tempNode->element->type << '>' << endl;
			for (int i = 0; i < tempNode->element->properties.size(); i++)
				cout << setw(depth * 4) << " " << left << "property " << i << " : ?"
					 << tempNode->element->properties[i].first
					 << " : "
					 << tempNode->element->properties[i].second << endl;
			cout << setw(depth * 4) << " " << tempNode->element->content << endl;
		}
	}

#endif

#include "private_type_implement.h"
#include "public_method_implement.h"
#include "iterator_public_method_implement.h"
