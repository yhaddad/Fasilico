#include <JCombination.hh>

using namespace std;
namespace fasilico{
  //namespace JFitter{
    /**
     * JCombination
     */
    //JCombination::JCombination* _me = 0; 
  
    JCombination* JCombination::instance(){
      if(_me == 0) 
	_me = new JCombination();
    
      return _me;
    }
    JCombination::JCombination(int n, int k, int offset, bool print)
    {
      _n = n;
      _k = k;
      if(_print_flag) 
	std::cout << "Number of combination C("<< n<<","<< k<<") :: "
		  <<  this->getNJCombination() << std::endl;
    
      for (int i = 0; i < n; ++i) { _data.push_back(i); }
      go(offset,k);
    }
    
    void JCombination::print(const std::vector<int>& v) {
      static int count = 0;
      cout << "combination no " << (++count) << ": [ ";
      for (unsigned int i = 0; i < v.size(); ++i) { cout << v[i] << " "; }
      cout << "] " << endl;
    }
  
    int JCombination::getNJCombination()
    {
      return TMath::Factorial(_n) / (TMath::Factorial(_n-_k) * TMath::Factorial(_k) );
    }
  
    void JCombination::go(int offset, int k) {
      if (k == 0) {
	if(_print_flag) this->print(_combination);
	_out.push_back(_combination);
	return;
      }
      for (unsigned int i = _offset; i <= _data.size() - k; ++i) {
	_combination.push_back(_data[i]);
	this->go(i+1, k-1);
	_combination.pop_back();
      }
    }
  
  //} // namespace JFitter
} // namespace
