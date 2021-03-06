#ifndef _TRVL_OPTNS_H
#define _TRVL_OPTNS_H

#include <iostream>
#include <vector>
#include <utility>


 using namespace std;

class TravelOptions{

  public:
	enum Relationship { better, worse, equal, incomparable};

  private:
	  struct Node {
		  double price;
		  double time;
		  Node *next;

		  Node(double _price=0, double _time=0, Node* _next=nullptr){
			  price = _price; time = _time; next = _next;
		  }

	  };

    /* TravelOptions private data members */
    Node *front;  // pointer for first node in linked list (or null if list is empty)
    int _size;

  public:
    // constructors
    TravelOptions() {
      front = nullptr;
      _size=0;
    }

    ~TravelOptions( ) {
      clear();
    }


   /**
   * func: clear
   * desc: Deletes all Nodes currently in the list
   * status:  DONE
   */
    void clear(){
    Node *p, *pnxt;
       p = front;
       while(p != nullptr) {
         pnxt = p->next;
         delete p;
         p = pnxt;
       }
       _size = 0;
       front = nullptr;
    }


   /**
   * func: size
   * desc: returns the number of elements in the list
   * status:  DONE
   */
    int size( ) const {
       return _size;
    }


    /**
    * func: compare
    * desc: compares option A (priceA, timeA) with option B (priceB, timeA) and
    *		returns result (see enum Relationship above):
    *
    *       There are four possible scenarios:
    *		- A and B are identical:  option A and option B have identical price and time: 
    *			 		ACTION:  return equal
    *		- A is better than B:  option A and B are NOT equal/identical AND 
    *					option A is no more expensive than option B AND
    *					option A is no slower than option B 
    *					ACTION:  return better
    *		- A is worse than B:  option A and B are NOT equal/identical AND 
    *					option A is at least as expensive as option B AND
    *					option A is no faster than option B 
    *					ACTION:  return worse
    *					NOTE:  this means B is better than A
    *		- A and B are incomparable:  everything else:  one option is cheaper and 
    *					      the other is faster.
    *					ACTION:  return incomparable 
    *
    * COMMENTS:  since this is a static function, there is no calling object.  
    *            To call it from a client program, you would do something like this:
    *
             TravelOptions::Relationship r;

             double pa, ta, pb, tb;

                // some code to set the four price/time variables

                r = TravelOptions::compare(pa, ta, pb, tb);
                if(r == TravelOptions::better) 
                    std::cout << "looks like option b is useless!" << std::endl;
                // etcetera
    * 
    * status: TODO 
    */          
    static Relationship compare(double priceA, double timeA, 
 					double priceB, double timeB) {

      if(priceA == priceB && timeA == timeB){
        return equal;
      }// if

      else if(priceA <= priceB && timeA <= timeB){
        return better;
      }// else if
      
      else if(priceA >= priceB && timeA >= timeB){

        return worse;
      }// else if

      return incomparable;  // placeholder

    }

  private:

    /**
     * func: compare(Node*, Node*)
     * desc: private utilty function for comparing two options given as 
     *       Node pointers.
     *
     * status: DONE
     *
     * COMMENT:  depends on public compare(double,double,double,double) being implemented.
     *           You might find this version handy when manipulating lists
     */
    static Relationship compare(Node *a, Node *b) {
       if(a==nullptr || b==nullptr) {
           std::cout << "ERR: compare(Node*,Node*);  null pointer passed!!! Whoops!" << std::endl;
           return incomparable;
       }
       return compare(a->price, a->time, b->price, b->time);
    }

    // dublicate_OR_Dominated
    // a helper function for is_pareto().
    // check if the two adjecent nodes are equal, better
    // or worse than each other and return bool value.
    //
    bool dublicate_OR_Dominated(Node* a) const{
      if(a == nullptr || a->next == nullptr)
        return false;

      Node* cur = a->next;
      while(cur != nullptr){
        
        if(compare(a, cur) == equal ||compare(a, cur) == worse ||compare(a, cur) == better){
          return true;
        }// if

        cur = cur->next;
      }// while

      return false;
    }// dublicate_exist

    // push_back
    // inserts a new node at the end of the linked list.
    //
    void push_back(Node* copy, Node* &cur) const{
      if(cur == nullptr){
        cur = new Node(copy->price, copy->time, nullptr);
        
      }
      else{
        Node* newNode = new Node(copy->price, copy->time, nullptr);
        (cur)->next = newNode;
        cur = newNode;
      }
    }

    void push_back(int price, int time, Node* &cur) const{
      if(cur == nullptr){
        cur = new Node(price, time, nullptr);
        
      }
      else{
        Node* newNode = new Node(price, time, nullptr);
        (cur)->next = newNode;
        cur = newNode;
      }
    }

    // remove_dominated
    // a helper function for insert_pareto_sorted()
    // remove dominted nodes after target
    //
    int remove_dominated(Node* &target) const{
      Node* prev = target;
      Node* cur = target->next;
      int removed = 0;

      while(cur!= nullptr){
        
        if (compare(target->price, target->time, cur->price, cur->time) == better){

          Node* temp = cur;
          cur = cur->next;
          prev->next = cur;
          delete temp;
          removed++;
          continue;
        }// if

        prev = cur;
        cur = cur->next;
      }// while

      return removed;
    }

    // getMaxTime
    // a helper function for join_plus_max().
    // return the maximum time of two nodes.
    int getMaxTime(Node* a, Node* b) const{

      if(a->time >= b->time)
        return a->time;

      return b->time;
    }

    // getSmallNode
    // a helper function for join_plus_max().
    // takes two nodes and returns the better/cheaper (smaller) node.
    //
    Node* getSmallNode(Node* a, Node* b, int &listNumber) const{
      if(compare(a,b) == better){
        listNumber = 1;
        return a;
      }// if 
        
      else if(compare(b,a) == better){
        listNumber = 2;
        return b;
      }// else if
        
      listNumber = 0;
      return nullptr;
    }

  public:
    
    

    

   /**
   * func: push_front
   * desc: Adds a <price,time> option to the front of the list (simple primitive for building lists)
   * status:  DONE
   */
    void push_front(double price, double time) {

      front = new Node(price, time, front);
      _size++;
    }

   /**
   * func: from_vec
   * desc: This function accepts a C++ standard libary vector of pair<double,double>.
   *       Each pair is interpreted as a <price,time> option and a TravelOptions object
   *       is containing exactly the same options as in the vector (and in the same order).
   *	
   * returns: a pointer to the resulting TravelOptions object
   * status:  DONE
   */
    static TravelOptions * from_vec(std::vector<std::pair<double, double> > &vec) {
	TravelOptions *options = new TravelOptions();

	for(int i=vec.size()-1; i>=0; i--) {
		options->push_front(vec[i].first, vec[i].second);
	}
	return options;
    }

   /**
   * func: to_vec
   * desc: Utility function which creates a C++ standard libary vector of pair<double,double>.
   *       and populates it with the options in the calling object (in the same order).
   *       As in from_vec the "first" field of each pair maps to price and the "second"
   *         field maps to time.
   *	
   * returns: a pointer to the resulting vector
   * status:  DONE
   */
    std::vector<std::pair<double, double>> * to_vec() const {
      std::vector<std::pair<double, double>> *vec = new std::vector<std::pair<double, double>>();
      Node *p = front;

      while(p != nullptr) {
           vec->push_back(std::pair<double,double>(p->price, p->time));
           p = p->next;
      }
      return vec;
    }


    /**
    * func: is_sorted
    * desc: we consider an option list sorted under the following conditions:
    *	
    *		- the options are in non-decreasing order of price AND
    *		- time is used as a tie-breaker for options with identical price.
    *
    *       For example, using the notation <price, time> to represent an option:
    *
    *             <5.1, 10.0>  must be before <5.6, 9.0>    (price is less, so time ignored)
    *             <6.2, 4.1>   must be AFTER  <6.2, 3.9>    (identical price; tie broken by
    *                                                         smaller time (3.9 in this case)).
    *
    *       If two or more options are identical in BOTH price and time, they are 
    *       indistinguishible and must appear as a consecutive "block" if the list is
    *       to be considered sorted.
    *
    * returns:  true if sorted by the rules above; false otherwise.
    *
    * Examples:
    *
    *   The option list below is sorted by our rules:
    *      [ <1, 7>, <2, 8>, <2, 9>, <3, 5>, <5, 8>, <5, 8>, <5, 9>, <6, 12> ]
    *
    *   The option list below is NOT sorted by our rules:
    *      [ <1, 7>, <2, 8>, <4, 3>, <3, 7>]
    *                                ^^^^^^ must be before <4,3>
    *
    *   The option list below is also NOT sorted by our rules:
    *      [ <1, 7>, <2, 8>, <2, 5>, <3, 7>]
    *                        ^^^^^^ must be before <2,8>
    * status: TODO
    */
    bool is_sorted()const{
      Node* cur = front;

      while(cur != nullptr && cur->next != nullptr){

        if(cur->price > cur->next->price)
          return false;

        else if(cur->price == cur->next->price && cur->time > cur->next->time)
            return false;

        cur = cur->next;
      }// while

	    return true;
    }


    /**
    * func: is_pareto
    * desc: returns true if and only if:
    *
    *        all options are distinct  (no duplicates)  AND
    *        none of the options are 'suboptimal' (i.e., for each option X, there DOES NOT EXIST
    *           any other option Y such that Y dominates X).  There are several equivalent
    *           ways of stating this property...
    *           
    * status: TODO
    *
    * REQUIREMENTS:
    *    - the list must be unaltered
    *    - no memory allocation, arrays, etc. allowed
    *    - RUNTIME:  quadratic in number of options n (i.e., O(n^2)).
    *
    * REMEMBER:  the list does not need to be sorted in order to be pareto
    */
    bool is_pareto() const{
      Node* cur = front;

      while(cur != nullptr){
        
        if(dublicate_OR_Dominated(cur))
          return false;
      
        cur = cur->next;
      }// while

    	return true;
    }

    /**
    * func: is_pareto_sorted() 
    * desc: returns true if and only if the list is:
    *    - STRICTLY INCREASING IN price AND
    *    - STRICTLY DECREASING IN time 
    *
    * REQUIREMENTS:
    *   RUNTIME:  linear in length of list n (i.e., O(n)).
    *
    * status:  TODO
    *
    * COMMENTS:  notice that because of the runtime requirement, you cannot simply do this:
    *
                   return is_sorted() && is_pareto();

    */
    bool is_pareto_sorted() const{

      if(_size == 0)
        return true;

      Node* cur = front;
      while(cur->next != nullptr){
      
        if(cur->price >= cur->next->price || cur->time <= cur->next->time)
          return false;
        
        cur = cur->next;
      }// while

	    return true;
    }// is_pareto_sorted

    /**
     * func: insert_sorted
     * preconditions:  given collection (calling object) must be sorted (but not necessarily
     *                   pareto).  If this is not the case, false is returned (code provided).
     *                   (returns true otherwise, after insertion complete).                 
     *                 
     * desc:  inserts option <price,time> (given as parameters) into option list (calling object)
     *          while keeping it sorted.  Recall:  ordering by price; tie-breaker is time.
     *
     * RUNTIME:  linear in length of list -- O(n).
     *
     * status: TODO
     *
     * NOTES/TIPS:  do this before insert_pareto_sorted; it is easier!  Remember, this one
     *     you don't have to think about pruning for this function -- just ordering.
     */

    bool insert_sorted(double price, double time) {
      
      if(!is_sorted()) return false;

      if(front == nullptr){
        front = new Node(price, time, nullptr);
        _size++;
        return true;
      }

      Node* cur = front;
      Node* nextP = cur->next;

      while(cur != nullptr){
        if(cur == front && (price < cur->price 
        || (price == cur->price && time < cur->time)) ){

          Node* newNode = new Node(price, time, cur);
          front = newNode;
          _size++;
          return true;
        }// if

        if(nextP != nullptr 
        && ((price > cur->price && price < nextP->price) 
        || (price == cur->price && time >= cur->time) || 
        (price == nextP->price && time < nextP->time))){

          Node* newNode = new Node(price, time, nextP);
          cur->next = newNode;
          _size++;
          return true;
        }// if

        if(nextP == nullptr && (price > cur->price 
        || (price == cur->price && time >= cur->time))){

          Node* newNode = new Node(price, time, nextP);
          cur->next = newNode;
          _size++;
          return true;
        }// if

        cur = nextP;
        nextP = nextP->next;
      }// while

       return true;
    }



    /**
     * func: insert_pareto_sorted
     * preconditions:  given collection (calling object) must be sorted AND pareto (pruned).
     *                 if this is not the case, false is returned.
     *                 (code segment for this test given).
     * desc: (assuming the list is sorted and pareto): if the option given by the parameters 
     *       price and time is NOT dominated by already existing options, the following results:
     *            - new option <price,time> is inserted maintaining the sorted property of the
     *                list, AND
     *            - any pre-existing options which are now suboptimal (i.e., dominated by the
     *                newly added option) are deleted.
     *       If the new option is suboptimal, the list is simply unchanged.
     *       In either case, true is returned (i.e., as long as the preconditions are met).
     *       
     * RUNTIME REQUIREMENT:  linear in the length of the list -- O(n)       
     *
     * REMEMBER:  
     *    If the new option is useless (dominated by a pre-existing option), the list is unchanged
     *         (but you still return true if preconditions are met).
     *    You must maintain sorted order and don't forget to deallocate memory associated
     *         with any deleted nodes.
     * status: TODO
     */
    bool insert_pareto_sorted(double price, double time) {
      
      if(!is_pareto_sorted())
       return false;

      if(front == nullptr){
        front = new Node(price, time, nullptr);
        _size++;
        return true;
      }// if

      Node* prev = nullptr;
      Node* cur = front;

      while(cur != nullptr){
        if(prev == nullptr && (price < cur->price && time > cur->time || compare(price,time, cur->price, cur->time) == better)){
          Node* newNode = new Node(price, time, cur);
          front = newNode;
          cur = newNode;
          _size++;
          _size -= remove_dominated(newNode);
          return true;
        }// if
        
        if(prev != nullptr && price > prev->price && time < prev->time && price < cur->price && time > cur->price ){
          Node* newNode = new Node(price, time, cur);
          prev->next = newNode;
          _size++;
          cur = newNode;
          _size -= remove_dominated(newNode);
          return true;
        }// if

        if(compare(price, time, cur->price, cur->time) == better){
          Node* newNode = new Node(price, time, cur);
          prev->next = newNode;
          cur = newNode;
          _size++;
          _size -= remove_dominated(newNode);
          return true;
        }// if

        if(cur->next == nullptr && price > cur->price && time < cur->time){
          Node* newNode = new Node(price, time, nullptr);
          cur->next = newNode;
          _size++;
          return true;
        }// if

        prev = cur;
        cur = cur->next;
      }// while

      return true;
    }

    

   /**
   * func: union_pareto_sorted
   * precondition:  calling object and parameter collections must both be sorted and pareto (if not, nullptr is returned).
   * desc: constructs "the sorted, pareto" union (as in set-union excluding dominated options) of the two collections and returns 
   *               it as a newly created object.
   * RUNTIME:  linear in the length of the two lists (suppose one list is of length n and the other is of length m, 
   *            then the overall runtime must be O(n+m))
   * COMMENTS:  REMEMBER:  after this operation, the collection must be both pareto and sorted.
   * TIPS:  Think about what must be the FIRST option in the sorted union (bootstrap) -- then think about what is the 
   *        candidate for the 2nd option (if any).  
   *        Remember:  a pareto-sorted list must be strictly increasing and price and strictly decreasing in time.
   * 
   * status:  TODO
   * 
   */
    TravelOptions * union_pareto_sorted(const TravelOptions &other)const{
       
      if(!is_pareto_sorted() || !other.is_pareto_sorted())
	      return nullptr;

      TravelOptions* u= new TravelOptions();

      Node* curU = nullptr;
      Node* cur1 = this->front;
      Node* cur2 = other.front;

      while(cur1 != nullptr || cur2 != nullptr){

        if(cur1 != nullptr && cur2 != nullptr){

          if(compare( cur1->price, cur1->time, cur2->price, cur2->time ) ==  better
          || compare( cur1->price, cur1->time, cur2->price, cur2->time ) ==  equal){
            
            if(u->front == nullptr){
              push_back(cur1, u->front);
              curU = u->front;
              u->_size++;
            }// if

            else{
              push_back(cur1, curU);
              u->_size++;
            }// else              
          }// if

          else if(compare( cur1->price, cur1->time, cur2->price, cur2->time ) ==  worse){

            if(u->front == nullptr){
              push_back(cur2, u->front);
              curU = u->front;
              u->_size++;
            }// if

            else
              push_back(cur2, curU);
              u->_size++;
          }// else if
            
          else if(cur1->price < cur2->price && cur1->time > cur2->time){

            if(u->front == nullptr){
              push_back(cur1, u->front);
              curU = u->front;
              push_back(cur2,curU);
              u->_size+=2;
            }// if

            else{

              if(compare(curU->price, curU->time, cur1->price, cur1->time) != better){
                push_back(cur1,curU);
                u->_size++;
              }

              if(compare(curU->price, curU->time, cur2->price, cur2->time) != better){
                push_back(cur2,curU);
                u->_size++;
              }
            }// else if
         
          }//else if

          else if(cur1->price > cur2->price && cur1->time < cur2->time){

            if(u->front == nullptr){

              push_back(cur2, u->front);
              curU = u->front;
              push_back(cur1,curU);
              u->_size+=2;
            }// if

            else{

              if(compare(curU->price, curU->time, cur2->price, cur2->time) != better){
                push_back(cur2,curU);
                u->_size++;
              }// if

              if(compare(curU->price, curU->time, cur1->price, cur1->time) != better){
                push_back(cur1,curU);
                u->_size++;
              }// if

            }// else
          }//else if

          cur1 = cur1->next;
          cur2 = cur2->next;
        }// if

        else if(cur1 == nullptr){

          if(u->front == nullptr){
              push_back(cur2, u->front);
              curU = u->front;
              u->_size++;
            }// if

            else{
              push_back(cur2,curU);
              u->_size++;
            }

          cur2 = cur2->next;
        }// else if

        else if(cur2 == nullptr){

          if(u->front == nullptr){
              push_back(cur1, u->front);
              curU = u->front;
              u->_size++;
            }// if

            else{
              push_back(cur1,curU);
              u->_size++;
            }// else

          cur1 = cur1->next;
        }// else if
        
      }// while

      return u;
    }
    
   /**
   * func:  prune_sorted
   * precondition:  given collection must be sorted (if not, false is returned).
   * desc: takes sorted list of options and removes dominated entries
   *         (and eliminates any duplicates).
   * RUNTIME:  linear in the length of the list (O(n))
   * COMMENTS:  the resulting list will be sorted AND pareto.
   * status:  TODO
   * 
   */
    bool prune_sorted(){
      
       if(!is_sorted()) 
        return false;

      if(front == nullptr)
        return true;

      Node* cur = front;
      Node* nextN = cur->next;

      while(nextN != nullptr){

        if(cur == front && compare(cur->price, cur->time, nextN->price, nextN->time) ==  worse){

          Node* temp = cur;
          cur = cur->next;
          front = cur;
          nextN = cur->next;

          delete temp;
          _size--;
          continue;
        }// if

        if(compare(cur->price, cur->time, nextN->price, nextN->time) ==  equal 
        || compare(cur->price, cur->time, nextN->price, nextN->time) ==  better){

          Node* temp = nextN;
          nextN = nextN->next;
          cur->next = nextN;

          delete temp;
          _size--;
          continue;
        }// if

        cur = nextN;
        nextN = cur->next;
      }// while

       return true;
    }


   /**
   * func: join_plus_plus
   * preconditions:  none -- both the calling object and parameter are just TravelOptions objects (not necessarily
   *                 sorted or pareto).
   * param: other; a reference to another to a list of TravelOptions (thus, there are two option lists: the calling 
   *               object and the parameter). 
   * desc: suppose you are planning a trip composed of two "legs":
   *
   *         first you travel from city X to city Y (this is part of your plan - you need to stop in city Y)
   *         then you continue from city Y to city Z
   *
   *       Let the calling object be the options for the X-to-Y leg and the parameter be the options for the
   *       Y-to-Z leg.
   *   
   *       Your job is to construct a pareto-sorted set of options for the entire trip and return the resulting
   *       TravelOptions object as a pointer.
   *
   *       In principle, any option <p1,t1> for the X-to-Y leg can be paird with any option <p2,t2> for the Y-to-Z leg.
   *       In such a pairing, the total price will be p1+p2 and the total time will be t1+t2, (i.e., option <p1+p2,t1+t2>).
   *       (This is why the operation is called join_plus_plus)
   *
   *       This function's  job is to determine the sorted-pareto list of options for the entire trip and return it
   *       as a pointer to the object.
   * returns:  a pointer to a TravelOptions object capturing all non-dominated options for the entire trip from X-to-Z
   *              (i.e., even though the given lists may not be sorted or pareto, the resulting list will be both).
   *
   * status:  TODO
   * RUNTIME:  no runtime requirement 
   *
   * TIPS:  
   *       Start by thinking about the "cross-product" of the two option lists (i.e., enumerating all pairs).
   *       Leverage some of the other operations in this assignment -- insert_pareto_sorted might be especially useful!
   *          (probably ought to implement any functions you plan on using first!).
   *       Don't overthink this one -- there is no runtime requirement after all.
   *
   * BOUNDARY CONDITIONS:  it is possible that one of both of the given option lists is empty!  The result is still 
   *    well-defined (also empty).  An empty option list is NOT the same as a null pointer though -- you should still return
   *   a pointer to a new TravelOptions object -- that object just happens to have an empty list.
   */
   TravelOptions * join_plus_plus(const TravelOptions &other) const{

    // pareto sorted joined list
    TravelOptions* jps = new TravelOptions();
    // non-pareto-sorted joined list
    TravelOptions* j = new TravelOptions();
    
    Node* cur1 = this->front;
    Node* cur2 = other.front;

    if(this->_size >= other._size){

      while(cur1 != nullptr){

        Node* temp = cur2;

        while(temp != nullptr){
          j->push_front(cur1->price + temp->price, cur1->time + temp->time);
          temp = temp->next;
        }// while

        if(cur2 == nullptr)
          j->push_front(cur1->price, cur1->time);

        cur1 = cur1->next;
      }// while
    }// if
    
    else{

      while(cur2 != nullptr){
        Node* temp = cur1;
        
        while(temp != nullptr){
          j->push_front(cur2->price + temp->price, cur2->time + temp->time);
          temp = temp->next;
        }// while

        if(cur1 == nullptr)
          j->push_front(cur2->price, cur2->time);
        cur2 = cur2->next;
      }// while
    }// else

    Node* curJ = j->front;

    while(curJ != nullptr){
      jps->insert_pareto_sorted(curJ->price, curJ->time);
      curJ = curJ->next;
    }// while

    return jps;  // placeholder to make the compiler happy
  }



   /**
   * func: join_plus_max
   * preconditions:  both the calling object and the parameter are sorted-pareto lists (if not, nullptr is returned).
   * desc: imagine a different scenario (vs. join_plus_plus):  you are a parent with two children -- one living in city A and
   *         the other living in city C -- and they are both coming home for the holidays (let's call your home C).  
   *       You have a pareto-sorted option list for the A-to-C trip (this is the calling object) and a pareto-sorted option list
   *         for the B-to-C trip.
   *       Consider a particular option for the A-to-C trip <p1, t1> and a particular option for the B-to-C trip <p2,t2>.
   *         Clearly, the total price for selecting these two options is p1+p2.
   *         However (unlike in the plus_plus case), adding the t1 and t2 doesn't make sense right (the trips are in "parallel").
   *         What you care about as a parent is when BOTH of your children are home.  This is determine by MAX(t1,t2).
   *         Thus, the resulting "composite" option in this case would be <p1+p2, MAX(t1,t2)> (hence the name join_plus_max).
   *       
   *       Your job is to construct the sorted-pareto "composite" option list capturing the options for getting both children home.
   *         The resulting TravelOptions object is returned as a pointer (recall if the preconditions are not met, 
   *         nullptr is returned).
   *       
   * RUNTIME:  let N and M be the lengths of the respective lists given; your runtime must be linear in N+M (O(N+M)).
   *
   * status:  TODO
   *
   * TIPS:
   *      This one will take some thought!  If the specified runtime is possible, the resulting option list cannot be too
   *      large right (at most N+M in length)?  But there NxM possible pairing of options.  So enummerating all pairs
   *      of options cannot be an option (as was suggested for the _plus_plus case).  
   *
   *      Start by figuring out what the FIRST (cheapest) option in the resulting list MUST be.  
   *      Remember that a sorted-pareto list must be strictly increasing in price and strictly decreasing in time.
   *      Now think about what might be the 2nd option in the list you are building.  It must cost more than the first AND
   *         take less time.  To be more concrete, suppose your first option has total price of $100 and results in child A
   *         traveling for 12 hours and child B traveling for 8 hours.  Does it make sense to spend more money on child B
   *         so they can get home in say 7 hours?  Think about it!  The MAX function is important!
   */
   TravelOptions * join_plus_max(const TravelOptions &other) const {
    if(!is_pareto_sorted() || !other.is_pareto_sorted())
	    return nullptr;
    TravelOptions* jpm = new TravelOptions();
    
    Node* cur1 = this->front;
    Node* cur2 = other.front;
    Node* curJ = jpm->front;

    // smallNode: copy the smallest better node from two nodes
    Node* smallNode = nullptr;
    
    // listNumber: figure out if smallNode saved belongs to list 1 or list 2, if neither return 0;
    int listNumber = 0;

    while(cur1 != nullptr || cur2 != nullptr){
      if(cur1 != nullptr && cur2 != nullptr){

        if(smallNode == nullptr){
          if(jpm->front == nullptr){
            push_back(cur1->price+cur2->price,getMaxTime(cur1,cur2),jpm->front);
            curJ = jpm->front;
            jpm->_size++;
          }// if

          else{
            push_back(cur1->price+cur2->price,getMaxTime(cur1,cur2), curJ);
            jpm->_size++;
          }// else       
          
          smallNode = getSmallNode(cur1,cur2, listNumber);
        }// if

        else{
          
          if(listNumber == 1 
          && compare(smallNode->price+cur2->price,getMaxTime(smallNode,cur2), cur1->price+cur2->price,getMaxTime(cur1,cur2)) != worse){

            push_back(smallNode->price+cur2->price,getMaxTime(smallNode,cur2), curJ);
            jpm->_size++;
          }// if

          else if(listNumber == 2 
          && compare(smallNode->price+cur1->price,getMaxTime(smallNode,cur1), cur1->price+cur2->price,getMaxTime(cur1,cur2)) != worse){

            push_back(smallNode->price+cur1->price,getMaxTime(smallNode,cur1), curJ);
            jpm->_size++;
          }// else if

          if(listNumber == 1
          && compare(cur1->price+cur2->price, getMaxTime(cur1,cur2),smallNode->price+cur2->price,getMaxTime(smallNode,cur2)) != worse){

            push_back(cur1->price+cur2->price, getMaxTime(cur1,cur2), curJ);
            jpm->_size++;

            smallNode = getSmallNode(cur1,cur2, listNumber);
          }// if
          
          else if(listNumber == 2
          && compare(cur1->price+cur2->price, getMaxTime(cur1,cur2),smallNode->price+cur1->price,getMaxTime(smallNode,cur1)) != worse){

            push_back(cur1->price+cur2->price, getMaxTime(cur1,cur2), curJ);
            jpm->_size++;

            smallNode = getSmallNode(cur1,cur2, listNumber);
          }// else if
        }// else

        cur1 = cur1->next;
        cur2 = cur2->next;
      }// if

      else if(cur1 == nullptr){
        push_back(cur2, curJ);
        jpm->_size++;

        cur2 = cur2->next;
      }// else if

      else{
        push_back(cur1, curJ);
        jpm->_size++;

        cur1 = cur1->next;
      }// else
    }// while

    return jpm;
   }

   /**
   * func: sorted_clone
   * desc: returns a sorted TravelOptions object which contains the same elements as the current object
   * status:  DONE [but relies on to do item insert_sorted]
   */
   TravelOptions * sorted_clone() {
	TravelOptions *sorted = new TravelOptions();
	Node *p = front;
	
	while(p != nullptr) {
	  sorted->insert_sorted(p->price, p->time);
	  p = p->next;
 	}
	return sorted;
   }

   /**
   * func: split_sorted_pareto
   * precondition:  given list must be both sorted and pareto (if not, nullptr is returned; 
   *    code already given).
   * desc: This function takes a sorted-pareto option list and splits into two option lists:
   *
   *        - the options with price less than or equal to max_price (if any) are retained in the calling
   *            object (and only those are retained in the calling object).
   *        - the other, more expensive options are used to populate a new TravelOptions object which
   *            is returned as a pointer (i.e., the expensive options are returned)..
   * 
   * returns: pointer to expensive options or nullptr if the calling object is not pareto-sorted.
   * RUNTIME:  linear in the length of the given list (O(n)).
   * ADDITIONAL REQUIREMENT:  for full credit you MAY NOT ALLOCATE ANY NEW NODES!  Think about it --
   *        suppose your given list has 100 options and 40 of them are below the max_price threshold; 
   *        the other 60 options end up in the returnd list.  Still a grand total of 100 options and 
   *        therefore 100 nodes.  So... there should be no reason to delete or allocate any nodes. 
   * status:  TODO
   */
   TravelOptions * split_sorted_pareto(double max_price) {
    if(!is_pareto_sorted())
	    return nullptr;
      
    TravelOptions* expensiveList = new TravelOptions();

    Node* cur = this->front;
    Node* prev = nullptr;
   
    while(cur != nullptr){

      if(max_price <= cur->price){
        expensiveList->front = cur;

        if(prev != nullptr)
          prev->next = nullptr;

        else
          this->front = nullptr;

        return expensiveList; 
      }// if

      prev = cur;
      cur = cur->next;
    }// while

    return expensiveList;  
   }

   /**
   * func: display
   * desc: prints a string representation of the current TravelOptions object
   * status:  DONE
   */
   void display(){
	printf("   PRICE      TIME\n");
	printf("---------------------\n");
	Node * p = front;
	
	while(p!=nullptr) {
		printf("   %5.2f      %5.2f\n", p->price, p->time);
		p = p->next;
	}
   }

  /**
   * func:  checksum
   * desc:  Performs and XOR of all node pointers and returns result as
   *        an unsigned int.
   * 
   * status: DONE
   *
   * NOTES: YOU MAY NOT TOUCH OR MODIFY THIS FUNCTION!!
   */
  unsigned long int checksum() const {
    unsigned long int s = 0;
    Node *p = front;

    while (p != nullptr)
    {
      s = s ^ ((unsigned long int)p);
      p = p->next;
    }
    return s;
  }

};

#endif
