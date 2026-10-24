#include<dkvs/skiplist.hpp>
#include<vector>
#include<set>
#include<iostream>
#include<cassert>
#include <random>

namespace{
void testSkipList(){
    using dkvs::SkipList;
    SkipList<int> sl;
    std::set<int> ref;
    std::mt19937 rng(42);

    for(int i=0;i<20000;++i){
        int op=static_cast<int>(rng()%3);
        int v=static_cast<int>(rng()%20000);
        if(op==0){
            assert(sl.insert(v)==ref.insert(v).second);
        }else if(op==1){
            assert(sl.erase(v)==ref.erase(v)>0);
        }else{
            assert(sl.contains(v)==ref.count(v)>0);
        }
         assert(sl.size() == ref.size());
    }

    std::vector<int> got=sl.toVector();
    std::vector<int> exp(ref.begin(),ref.end());
    assert(got==exp);
    std::cout<<"对拍通过, size="<<sl.size()<<"\n";
}
}

int main(){
    testSkipList();
    return 0;
}
