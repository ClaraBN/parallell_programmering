#ifndef lacpp_sorted_list_fine_mutex_hpp
#define lacpp_sorted_list_fine_mutex_hpp lacpp_sorted_list_fine_mutex_hpp

/* a sorted list implementation by David Klaftenegger, 2015
 * please report bugs or suggest improvements to david.klaftenegger@it.uu.se
 */

/* struct for list nodes */
template<typename T>
struct node {
	T value;
	node<T>* next;
};

template<typename T>
struct MyHash {
    std::size_t operator()(node<T>* const& n) const noexcept{
        std::size_t h1 = std::hash<T>{}(n->value);
        std::size_t h2 = std::hash<node<T>*>{}(n->next);
        return h1 ^ (h2 << 1); 
    }
};

/* non-concurrent sorted singly-linked list */
template<typename T>
class sorted_list {
	node<T>* first = nullptr;
	

	public:
		/* default implementations:
		 * default constructor
		 * copy constructor (note: shallow copy)
		 * move constructor
		 * copy assignment operator (note: shallow copy)
		 * move assignment operator
		 *
		 * The first is required due to the others,
		 * which are explicitly listed due to the rule of five.
		 */
		sorted_list() = default;
		sorted_list(const sorted_list<T>& other) = default;
		sorted_list(sorted_list<T>&& other) = default;
		sorted_list<T>& operator=(const sorted_list<T>& other) = default;
		sorted_list<T>& operator=(sorted_list<T>&& other) = default;
		~sorted_list() {
			while(first != nullptr) {
				remove(first->value);
			}
		}
		/* insert v into the list */
		void insert(T v) {
			
			/* first find position */
			node<T>* pred = nullptr;
			// first->fine_mutex.lock();
			node<T>* succ = first;
			// first->fine_mutex.unlock();

			while(succ != nullptr && succ->value < v) {
				// succ->fine_mutex.lock();
				pred = succ;
				succ = succ->next;
				// succ->fine_mutex.unlock();
			}
			
			/* construct new node */
			node<T>* current = new node<T>();
			current->value = v;

			/* insert new node between pred and succ */
			// succ->fine_mutex.lock();
			current->next = succ;
			// succ->fine_mutex.unlock();
			
			if(pred == nullptr) {
				/* I detta fall är first och succ samma för vi vill sätta in i början av listan,
				kan man låsa två ggr efter varandra? eller är succ en kopia av first och därmed sitt egna mutex? */
				// first->fine_mutex.lock();
				first = current;
				// first->fine_mutex.unlock();
			} else {
				// pred->fine_mutex.lock();
				pred->next = current;
				// pred->fine_mutex.unlock();
			}
			std::cout << MyHash<T>{}(current) << "hopefully hash for current\n" << MyHash<T>{}(succ) << "hopefully hash for succ\n";
			
		}

		void remove(T v) {
			/* first find position */

			/*                                                                     */
			/* Jag har börjat här XP jag har skrivit kommentarer vi varje (un)lock */
			/*          men även lämnat en kommentar över en tanke ovan ^          */
			/*                                                                     */

			
			node<T>* pred = nullptr;
			// lock
			// first->fine_mutex.lock();
			node<T>* current = first;
			// unlock
			// first->fine_mutex.unlock();
			while(current != nullptr && current->value < v) {
				// lock
				// current->fine_mutex.lock();
				pred = current;
				current = current->next;
				// unlock
				// current->fine_mutex.unlock();
			}
			if(current == nullptr || current->value != v) {
				/* v not found */
				return;
			}
			/* remove current */

			/* om jag fattat rätt sker if-satsen bara om vi raderar en lista där
			   önskat värde är första värdet, så vi borde bara låsa current->next. 
			   men om insert sätter in en innan first, kan det skapa problem? Lämnar 
			   detta som en tanke utifall det är problem längre fram.. (^_^;)*/
			if(pred == nullptr) {
				// lock
				// current->next->fine_mutex.lock();
				first = current->next;
				// unlock
				// current->next->fine_mutex.unlock();
			} else {
				// lock (times two)
				// pred->fine_mutex.lock();
				// current->next->fine_mutex.lock();
				pred->next = current->next;
				// unlock (times two)
				// current->next->fine_mutex.unlock();
				// pred->fine_mutex.unlock();
			}
			delete current;
			
		}

		/* count elements with value v in the list */
		std::size_t count(T v) {
			
			std::size_t cnt = 0;
			/* first go to value v */
			// first->fine_mutex.lock();
			node<T>* current = first;
			// first->fine_mutex.unlock();
			while(current != nullptr && current->value < v) {
				/* jag låser curren->next eftersom det är den vi inte har gjort något med ännu*/
				// current->next->fine_mutex.lock();
				current = current->next;
				// current->next->fine_mutex.unlock();
			}
			/* count elements */
			while(current != nullptr && current->value == v) {
				cnt++;
				// current->next->fine_mutex.lock();
				current = current->next;
				// current->next->fine_mutex.unlock();
			}
			
			return cnt;
			
		}
};

#endif // lacpp_sorted_list_fine_mutex_hpp
