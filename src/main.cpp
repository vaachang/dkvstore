#include <dkvs/skiplist.hpp>
#include <iostream>
int main(){
    using dkvs::SkipList;
    SkipList<int> demo;
    for(int x:{5,3,8,1,9,2,7}){
        demo.insert(x);
    }
    std::cout<<"跳表示例结构：\n";
    demo.print();
    return 0;
}
