#pragma once

template <class MyData, class MyKey>
class MyTree {

protected:
	class MyNode {
	public:
		MyKey Key;
		MyData Data;
		int n;
		MyNode* Left;
		MyNode* Right;

		MyNode(MyKey K, MyData D) { //Конструктор по умлчанию
			Key = K;
			Data = D;
			n = 1;
			Left = nullptr;
			Right = nullptr;
		}
	};
	friend class MyNode;

private:
	int NumberOfNodes;	//Количество узлов вдереве
	MyNode* TreeRoot;	//Корень дерева

public:

	//ИТЕРАТОР

	class Iterator
	{
	private:
		MyTree* Ptr;	//указатель на объект коллекции
		MyNode* Cur;	//указатель на текущий элемент коллекции

	public:

		//конструктор
		Iterator(MyTree<MyKey, MyData>& Tree) {
			Ptr = &Tree;
			Cur = nullptr;
		}

		//установка на элемент с наименьшим ключом c использованием рекурсии
		void begin() {
			if (Ptr->TreeRoot != nullptr) {
				Cur = go_to_begin(Ptr->TreeRoot);
			}
			else {
				throw - 1;
			}
			return;
		}

		MyNode* go_to_begin(MyNode* Cur) {
			if (Cur->Left != nullptr) {
				return go_to_begin(Cur->Left);
			}
		}

		//установка на следующий
		void next() {
			if (Cur != nullptr) {
				Cur = Ptr->BST_Successor(Ptr->TreeRoot, Cur);
			}
			else {
				throw - 1;
			}
			return;
		}

		//проверка состояния итератора
		bool is_off() const {
			return (Cur == nullptr);
		}

		//доспуп к данным текущего элемента
		MyData& operator*() {
			if (Cur != nullptr)
				return Cur->Data;
			else
				throw - 1;
		}
	};
	friend class Iterator;

	//ОБРАТНЫЙ ИТЕРАТОР

	class ReverseIterator
	{
	private:
		MyTree* Ptr;	//указатель на объект коллекции
		MyNode* Cur;	//указатель на текущий элемент коллекции
	public:
		//конструктор
		ReverseIterator(MyTree<MyKey, MyData>& Tree) {
			Ptr = &Tree;
			Cur = nullptr;
		}

		//установка на элемент с наибольшим ключом c использованием рекурсии
		void rbegin() {
			if (Ptr->TreeRoot != nullptr) {
				Cur = go_to_rbegin(Ptr->TreeRoot);
			}
			else {
				throw - 1;
			}
			return;
		}

		MyNode* go_to_rbegin(MyNode* Cur) {
			if (Cur->Right != nullptr) {
				return go_to_rbegin(Cur->Right);
			}
		}

		//проверка состояния итератора
		bool is_off() const {
			return (Cur == nullptr);
		}

		//установка на предыдущий по ключу
		void prev() {
			if (Cur != nullptr) {
				Cur = Ptr->BST_Predcessor(Ptr->TreeRoot, Cur);
			}
			else {
				throw - 1;
			}
			return;
		}

		//доспуп к данным текущего элемента
		MyData& operator*() {
			if (Cur != nullptr)
				return Cur->Data;
			else
				throw - 1;
		}
	};
	friend class ReverseIterator;

	//ТЕХНИЧЕСКИЕ ФУНКЦИИ

	//неустановленный прямой итератор
	void end() {
		Iterator itr;
		itr.Cur = nullptr;
		return;
	}

	//неустановленный обратный итератор
	void rend() {
		Iterator ritr;
		ritr.Cur = nullptr;
		return;
	}

	//Конструктор дерева
	MyTree() {
		TreeRoot = nullptr;
		NumberOfNodes = 0;
	}

	//Конструктор копирования дерева
	MyTree(const MyTree<MyData, MyKey>& OldTree) {
		TreeRoot = nullptr;
		NumberOfNodes++;
		Copy(OldTree.NumberOfNodes);
	}

	void Copy(MyNode* r)
	{
		if (r == nullptr)
			return;
		int tmp = 0;
		Insert(r->key, r->data, tmp);
		Copy(r->Left);
		Copy(r->Right);
	}

	//деструктор
	~MyTree() {
		Delete();
	}

	//Удаление дерева
	void Delete() {
		if (TreeRoot == nullptr) return;
		MyNode* del, * prev = nullptr, * temp = TreeRoot;
		while (1) {
			while (temp->Left || temp->Right) {
				while (temp->Left) { prev = temp; temp = temp->Left; }
				while (temp->Right) { prev = temp; temp = temp->Right; }
			}
			del = temp;
			if (del == TreeRoot) {
				TreeRoot = nullptr;
				NumberOfNodes = 0;
				delete del;
				return;
			}
			temp = TreeRoot;//temp->parent;
			if (prev->Left == del)prev->Left = nullptr;
			else prev->Right = nullptr;
			delete del;
		}
	}

	//ФУНКЦИИ ДОСТУПНЫЕ ПОЛЬЗОВАТЕЛЮ

	//Опрос размера списка
	int AskForSize() {
		return NumberOfNodes;
	}

	//Проверка на пустоту
	bool EmptyCheck() {
		return(NumberOfNodes == 0);
	}

	//Чтение данных с заданным ключом (итерации)
	MyData RND_Read(MyKey Key, int& nn) {
		MyNode* Root = TreeRoot;
		while (Root != nullptr) {
			if (Root->Key == Key) {
				return Root->Data;
			}
			else if (Key < Root->Key) {
				nn++;
				Root = Root->Left;
			}
			else if (Key > Root->Key) {
				nn++;
				Root = Root->Right;
			}
		}
		throw - 1;
	}

	//Вставка
	
	//Вставка данных по ключу рандомизированная итеративная
	bool RND_Insert(MyKey Key, MyData Data, int& nn) {
		MyNode* Root = TreeRoot;
		MyNode* Pred = TreeRoot;
		if (TreeRoot == nullptr) {//вставка в начало
			TreeRoot = new MyNode(Key, Data);
			NumberOfNodes++;
			return true;
		}
		while (Root != nullptr) {
			if (rand() < (RAND_MAX / (Root->n + 1))) {//условие вставки в корень
				return Root_Insert(Pred, Root, Key, Data, nn);
			}
			else {
				Pred = Root;
				if (Key == Root->Key) {
					go_back_ins(Key);
					return false;
				}
				else if (Key < Root->Key) {
					nn++;
					Root->n++;
					Root = Root->Left;
				}
				else if (Key > Root->Key) {
					nn++;
					Root->n++;
					Root = Root->Right;
				}
			}
		}
		if (Key < (Pred->Key)) {
			Pred->Left = new MyNode(Key, Data);
		}
		else if (Key > (Pred->Key)) {
			Pred->Right = new MyNode(Key, Data);
		}
		NumberOfNodes++;
		return true;
	}
	
	bool Root_Insert(MyNode* & LocalPred, MyNode* & LocalRoot, MyKey Key, MyData Data, int& nn) {//вставка в корень
		MyNode* Root = LocalRoot;
		MyNode* Pred = LocalRoot;
		MyNode* Last;
		int Length = 1;
		int i = 0;
		MyNode** Array;
		
		if (LocalRoot == nullptr) {//вставка в начало
			LocalRoot = new MyNode(Key, Data);
			NumberOfNodes++;
			return true;
		}
		while (Root != nullptr) {
			Pred = Root;
			nn++;
			if (Key == Root->Key) {//данный ключ уже существует
				go_back_ins(Key);
				return false;
			}
			else if (Key < Root->Key) {//данный ключ меньше ключа в корне
				Root->n++;
				Length++;
				Root = Root->Left;
			}
			else if (Key > Root->Key) {//данный ключ больше ключа в корне
				Root->n++;
				Length++;
				Root = Root->Right;
			}
		}
		if (Key < (Pred->Key)) {//вставка в конец ветви
			Pred->Left = new MyNode(Key, Data);
		}
		else if (Key > (Pred->Key)) {
			Pred->Right = new MyNode(Key, Data);
		}
		NumberOfNodes++;
		
		Array = new MyNode * [Length+1];//заполнение массива узлов, которые нужно повернуть
		Root = LocalRoot;
		while (Root != nullptr) {
			nn++;
			Array[i] = Root;
			if (Key < Root->Key) {//данный ключ меньше ключа в корне
				i++;
				Root = Root->Left;
			}
			else {//данный ключ больше ключа в корне
				i++;
				Root = Root->Right;
			}
		}
		
		i = i-2;

		Last = Array[i + 1];

		while (i > 0) {//поворот узлов
			nn++;
			if (Array[i]->Key > Array[i - 1]->Key) {
				if (Array[i + 1]->Key > Array[i]->Key) {
					Array[i - 1]->Right = Rotate_Left(Array[i]);
					Last = Array[i - 1]->Right;
				}
				else {
					Array[i - 1]->Right = Rotate_Right(Array[i]);
					Last = Array[i - 1]->Right;
				}	
			}
			else {
				if (Array[i + 1]->Key > Array[i]->Key) {
					Array[i - 1]->Left = Rotate_Left(Array[i]);
					Last = Array[i - 1]->Left;
				}
				else {
					Array[i - 1]->Left = Rotate_Right(Array[i]);
					Last = Array[i - 1]->Left;
				}
			}
			i--;
		}
		
		if (TreeRoot == LocalRoot) {//если корень - корень всего дерева
			if (Last->Key > LocalRoot->Key) {
				MyNode* y = new MyNode(TreeRoot->Key, TreeRoot->Data);
				y->Left = TreeRoot->Left;
				y->Right = Last->Left;
				Last->Left = y;
				TreeRoot = Last;
				fixsize(TreeRoot->Left);
				fixsize(TreeRoot);
			}
			else {
				MyNode* y = new MyNode(TreeRoot->Key, TreeRoot->Data);
				y->Right = TreeRoot->Right;
				y->Left = Last->Right;
				Last->Right = y;
				TreeRoot = Last;
				fixsize(TreeRoot->Right);
				fixsize(TreeRoot);
			}
		}
		else {//если корень не совпадает с корнем дерева
			if (Array[i]->Key > LocalPred->Key) {
				if (Last->Key > Array[i]->Key) {
					LocalPred->Right = Rotate_Left(Array[i]);
				}
				else {
					LocalPred->Right = Rotate_Right(Array[i]);
				}
			}
			else {
				if (Last->Key > Array[i]->Key) {
					LocalPred->Left = Rotate_Left(Array[i]);
				}
				else {
					LocalPred->Left = Rotate_Right(Array[i]);
				}
			}
		}
		
		return true;
	}

	MyNode* Rotate_Right(MyNode* Root) {//поворот вправо
		MyNode* x = Root->Left;
		if (x == nullptr) return Root;
		Root->Left = x->Right;
		x->Right = Root;
		fixsize(x->Right);
		fixsize(x);
		return x;
	}

	MyNode* Rotate_Left(MyNode* Root) {//поворот влево
		MyNode* x = Root->Right;
		if (x == nullptr) return Root;
		Root->Right = x->Left;
		x->Left = Root;
		fixsize(x->Left);
		fixsize(x);
		return x;
	}

	
	int getsize(MyNode* Root) // обертка для поля size, работает с пустыми деревьями (t=NULL)
	{
		if (Root==nullptr) return 0;
		return Root->n;
	}

	void fixsize(MyNode* Root) // установление корректного размера дерева
	{
		Root->n = getsize(Root->Left) + getsize(Root->Right) + 1;
	}

	// Конец вставки
	
	// Итеративное возвращение при попытке вставить существующий ключ
	void go_back_ins(MyKey Key) {
		MyNode* Root = TreeRoot;
		while (Root->Key != Key) {
			if (Key < Root->Key) {
				Root->n--;
				Root = Root->Left;
			}
			else if (Key > Root->Key) {
				Root->n--;
				Root = Root->Right;
			}
		}
		return;
	}

	// Итеративное возвращение при попытке удалить несуществующий ключ
	void go_back_del(MyKey Key) {
		MyNode* Root = TreeRoot;
		while (Root != nullptr) {
			if (Key < Root->Key) {
				Root->n++;
				Root = Root->Left;
			}
			else if (Key > Root->Key) {
				Root->n++;
				Root = Root->Right;
			}
		}
		return;
	}

	//Рандомизированное удаление по ключу
	bool RND_Delete(MyKey Key, int& nn) {
		MyNode* Root = TreeRoot;
		MyNode* Pred = TreeRoot;
		MyNode* x = nullptr;
		while ((Root != nullptr) && (Root->Key != Key)) {
			Pred = Root;
			nn++;
			if (Key < Root->Key) {
				Root->n--;
				Root = Root->Left;
			}
			else {
				Root->n--;
				Root = Root->Right;
			}
		}
		if (Root == nullptr) {//Удаляемый не найден
			go_back_del(Key);
			return false;
		}
		x = RND_Join(Root->Left, Root->Right);
		if (Root == TreeRoot) {
			TreeRoot = x;
			NumberOfNodes--;
			return true;
		}
		Delete_Node(Root);
		if (Pred->Key > Key) {
			Pred->Left = x;
		}
		else {
			Pred->Right = x;
		}
		NumberOfNodes--;
		return true;
	}

	void Delete_Node(MyNode* Root) {
		delete Root;
	}

	//Рандомизированное объединение поддеревьев
	MyNode* RND_Join(MyNode* A, MyNode* B) {
		if (A == nullptr) {
			return B;
		}
		if (B == nullptr) {
			return A;
		}
		if ((rand() / (RAND_MAX / (A->n + B->n + 1))) < A->n) {
			A->Right = RND_Join(A->Right, B);
			fixN(A);
			return A;
		}
		else {
			B->Left = RND_Join(A, B->Left);
			fixN(B);
			return B;
		}
	}

	void fixN(MyNode* h) {
		if (h == nullptr) return;
		if (h->Left == nullptr && h->Right == nullptr) { h->n = 1; return; }
		else {
			if (h->Left == nullptr) { h->n = h->Right->n + 1; return; }
			if (h->Right == nullptr) { h->n = h->Left->n + 1; return; }
		}
		h->n = h->Left->n + h->Right->n + 1;
	}

	//Опрос наличия заданного ключа (итерации)
	bool Iteration_Key_Check(MyKey Key) {
		MyNode* Root = TreeRoot;
		while (Root != nullptr) {
			if (Root->Key == Key) {
				return true;
			}
			if (Key < Root->Key) {
				Root = Root->Left;
			}
			else if (Key > Root->Key) {
				Root = Root->Right;
			}
		}
		return false;
	}

	void StartPrint() {
		Print(TreeRoot);
	}

	//tLR вывод дерева
	void Print(MyNode* Root) {
		if (Root == nullptr) {
			return;
		}
		std::cout << Root->Key << std::endl;
		Print(Root->Left);
		Print(Root->Right);
	}

	//печать дерева
	void PrintTree() {
		Show(TreeRoot, 0);
	}

	void Show(MyNode* Root, int level) {
		if (Root == nullptr) {
			return;
		}
		Show(Root->Right, level + 1);
		for (int i = 0; i <= 3 * level; i++) {
			std::cout << "	";
		}
		std::cout << Root->Key << " " << Root->n << std::endl;
		Show(Root->Left, level + 1);
	}

	//Поиск следующего по ключу узла
	MyNode* BST_Successor(MyNode* TRoot, MyNode* Root) {
		if (Root->Right != nullptr) {
			return Min(Root->Right);
		}
		else {
			return L_Successor(TRoot, Root);
		}
	}

	MyNode* Min(MyNode* Root) {
		if (Root == nullptr) {
			return nullptr;
		}
		if (Root->Left != nullptr) {
			return Min(Root->Left);
		}
		return Root;
	}

	MyNode* L_Successor(MyNode* TRoot, MyNode* Root) {
		MyNode* LS;
		if (Root == TRoot) {
			return nullptr;
		}
		if (Root->Key < TRoot->Key) {
			LS = L_Successor(TRoot->Left, Root);
			if (LS != nullptr) {
				return LS;
			}
			else {
				return TRoot;
			}
		}
		else {
			return L_Successor(TRoot->Right, Root);
		}
	}

	//поиск предыдущего по ключу узла
	MyNode* BST_Predcessor(MyNode* TRoot, MyNode* Root) {
		if (Root->Left != nullptr) {
			return Max(Root->Left);
		}
		else {
			return R_Parent(TRoot, Root);
		}
	}

	MyNode* Max(MyNode* Root) {
		if (Root == nullptr) {
			return nullptr;
		}
		if (Root->Right != nullptr) {
			Root = Max(Root->Right);
		}
		return Root;
	}

	MyNode* R_Parent(MyNode* TRoot, MyNode* Root) {
		MyNode* rp;
		if (TRoot == Root) {
			return nullptr;
		}
		if (Root->Key > TRoot->Key) {
			rp = R_Parent(TRoot->Right, Root);
			if (rp != nullptr) {
				return rp;
			}
			else {
				return TRoot;
			}
		}
		else {
			return R_Parent(TRoot->Left, Root);
		}
	}

	//Удаление элемента по ключу (не рандомизированное)
	bool BST_Delete(MyKey Key, int& nn) {
		MyNode* Root = TreeRoot;
		MyNode* Pred = nullptr;
		MyNode* x = nullptr;
		MyNode* y = nullptr;
		while ((Root != nullptr) && (Root->Key != Key)) {
			Pred = Root;
			nn++;
			if (Key < Root->Key) {
				Root->n--;
				Root = Root->Left;
			}
			else {
				Root->n--;
				Root = Root->Right;
			}
		}
		if (Root == nullptr) {//Удаляемый не найден
			go_back_del(Key);
			return false;
		}
		Pred = nullptr;
		if ((Root->Left == nullptr) && (Root->Right == nullptr)) {
			x = nullptr;
		}
		else if (Root->Left == nullptr) {
			x = Root->Right;
		}
		else if (Root->Right == nullptr) {
			x = Root->Left;
		}
		else {
			Pred = Root;
			y = Root->Right;
			while (y->Left != nullptr) {
				Pred = y;
				y = y->Left;
			}
			Root->Key = y->Key;
			Root->Data = y->Data;
			x = y->Right;
			Root = y;
		}
		if (Pred == nullptr) {
			Root = x;
		}
		else {
			if (Root->Key < Pred->Key) {
				Pred->Left = x;
			}
			else {
				Root->Right = x;
			}
		}
		Delete_Node(Root);
		NumberOfNodes--;
		return true;
	}





	//////////////////////////////////////////////////////////////////////////////////

	//Вставка данных по ключу в BST дерево
	bool BST_Insert(MyKey Key, MyData Data, int& nn) {
		MyNode* Root = TreeRoot;
		MyNode* Pred = nullptr;
		if (TreeRoot == nullptr) {//вставка в начало
			TreeRoot = new MyNode(Key, Data);
			NumberOfNodes++;
			return true;
		}
		else {
			while (Root != nullptr) {
				Pred = Root;
				if (Key == Root->Key) {
					Root->Data = Data;
					go_back_ins(Key);
					return false;
				}
				else if (Key < Root->Key) {
					nn++;
					Root->n++;
					Root = Root->Left;
				}
				else if (Key > Root->Key) {
					nn++;
					Root->n++;
					Root = Root->Right;
				}
			}
		}
		if (Key < (Pred->Key)) {
			Pred->Left = new MyNode(Key, Data);
		}
		else {
			Pred->Right = new MyNode(Key, Data);
		}
		NumberOfNodes++;
		return true;
	}

	MyNode* rotateright(MyNode* p) // правый поворот вокруг узла p
	{
		MyNode* q = p->Left;
		if (q == nullptr) return p;
		p->Left = q->Right;
		q->Right = p;
		q->n = p->n;
		fixsize(p);
		return q;
	}

	MyNode* rotateleft(MyNode* q) // левый поворот вокруг узла q
	{
		MyNode* p = q->Right;
		if (p == nullptr) return q;
		q->Right = p->Left;
		p->Left = q;
		p->n = q->n;
		fixsize(q);
		return p;
	}

	// Рандомизированная рекурсивная вставка из методички
	MyNode* R_Insert(MyNode*& Root, MyKey Key, MyData Data, bool& inserted, int& nn) {
		bool ins = true;
		nn++;
		if (Root == nullptr) {
			Root = new MyNode(Key, Data);
			inserted = true;
			return Root;
		}
		if (rand() < (RAND_MAX / (Root->n + 1))) {
			Root = BST_Root_Insert(Root, Key, Data, ins, nn);
			inserted = ins;
			return Root;
		}
		if (Key == Root->Key) {
			inserted = false;
			return Root;
		}
		if (Key < Root->Key) {
			Root->Left = R_Insert(Root->Left, Key, Data, ins, nn);
		}
		else {
			Root->Right = R_Insert(Root->Right, Key, Data, ins, nn);
		}
		inserted = ins;
		if (inserted == true) {
			Root->n = Root->n + 1;
		}
		return Root;
	}

	//Вставка в корень, вспомогательная функция для R_Insert
	MyNode* BST_Root_Insert(MyNode*& Root, MyKey Key, MyData Data, bool& inserted, int& nn) {
		bool ins;
		nn++;
		if (Root == nullptr) {
			inserted = true;
			Root = new MyNode(Key, Data);
			return Root;
		}
		if (Root->Key == Key) {
			inserted = false;
			return Root;
		}
		if (Key < Root->Key) {
			Root->n++;
			Root->Left = BST_Root_Insert(Root->Left, Key, Data, ins, nn);
			inserted = ins;
			if (ins == true) {
				return R(Root);
			}
			else {
				return Root;
			}
		}
		else {
			Root->n++;
			Root->Right = BST_Root_Insert(Root->Right, Key, Data, ins, nn);
			inserted = ins;
			if (ins == true) {
				return L(Root);
			}
			else {
				return Root;
			}
		}
	}

	// Левый поворот вокруг Root
	MyNode* L(MyNode*& Root) {
		if (Root == nullptr) {
			return nullptr;
		}
		else {
			MyNode* x = Root->Right;
			Root->Right = x->Left;
			x->Left = Root;
			fixsize(x->Left);
			fixsize(x);
			return x;
		}
	}

	// Правый поворот вокруг Root
	MyNode* R(MyNode*& Root) {
		if (Root == nullptr) {
			return nullptr;
		}
		else {
			MyNode* x = Root->Left;
			Root->Left = x->Right;
			x->Right = Root;
			fixsize(x->Right);
			fixsize(x);
			return x;
		}
	}


	MyNode* insert(MyNode* p, MyKey Key, MyData Data, int& nn) // рандомизированная вставка нового узла с ключом k в дерево p
	{
		if (p == nullptr) return new MyNode(Key, Data);
		if //(rand() % (p->n + 1) == 0)
			(rand() < (RAND_MAX / (p->n + 1)))
			return insertroot(p, Key, Data, nn);
		if (p->Key > Key)
			p->Left = insert(p->Left, Key, Data, nn);
		else
			p->Right = insert(p->Right, Key, Data, nn);
		fixsize(p);
		return p;
	}

	MyNode* insertroot(MyNode* p, MyKey Key, MyData Data, int& nn) // вставка нового узла с ключом k в корень дерева p 
	{
		if (p == nullptr) return new MyNode(Key, Data);
		if (Key < p->Key)
		{
			p->Left = insertroot(p->Left, Key, Data, nn);
			return rotateright(p);
		}
		else
		{
			p->Right = insertroot(p->Right, Key, Data, nn);
			return rotateleft(p);
		}
	}





};


