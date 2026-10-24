#include <cassert>
#include <iostream>
#include <memory>
#include <random>
#include <set>
#include <vector>

template <typename T,typename Compare = std::less<T>>
class SkipList{
    private:
        struct Node{
            T value;
            std::vector<Node*> next;   //next[i]表示第i层的后继结点

            Node(const T& v,int level): value(v),next(level, nullptr){}
        };

        static constexpr int kMaxLevel  = 32;   //最大层数，支持4^32个元素
        static constexpr double kP      = 0.25; //每层晋升概率

        std::unique_ptr<Node>   head_;  //哨兵头结点
        int                     level_; //当前有效层数
        std::size_t             size_;  //元素个数
        Compare                 cmp_;   //比较器：cmp(a,b)为true，表示a<b
        std::mt19937            rng_;
        std::uniform_real_distribution<double> dist_{0.0, 1.0};

        //随机生成新节点的层数
        int randomLevel(){
            int lvl=1;
            while(lvl<kMaxLevel && dist_(rng_)<kP) ++lvl;
            return lvl;
        }

        //对每一层i，找出最后一个满足node->value < v的节点，写入prev[i]
        void findPredecessors(const T& v, std::vector<Node*>& prev) const {
            Node* cur = head_.get();
            for(int i=level_ -1 ;i>=0;--i){ //下移
                while(cur->next[i]&&cmp_(cur->next[i]->value,v)){
                    cur=cur->next[i]; //右移
                }
                prev[i]=cur;
            }
        }
    public:
        explicit SkipList(Compare cmp=Compare())
            : head_(std::make_unique<Node>(T{},kMaxLevel)),
              level_(1),
              size_(0),
              cmp_(std::move(cmp)),
              rng_(std::random_device{}()){}

        ~SkipList(){
            clear();
        }

        SkipList(const SkipList&)=delete;
        SkipList& operator=(const SkipList&)=delete;
        SkipList(SkipList&&)=delete;
        SkipList& operator=(SkipList&&)=delete;

        std::size_t size()const noexcept{
            return size_;
        }

        bool empty()const noexcept{
            return size_==0;
        }

        bool contains(const T& v) const {
            Node* cur=head_.get();
            for(int i=level_-1;i>=0;i--){
                while(cur->next[i]&& cmp_(cur->next[i]->value,v)){
                    cur=cur->next[i];
                }
            }
            cur=cur->next[0]; //保证cur->value >= v，如果cur不为空
            return cur!=nullptr&&!cmp_(v,cur->value); //v>=cur->value => v==cur->value
        }

        bool insert(const T& v){
            std::vector<Node*> prev(kMaxLevel,head_.get());
            findPredecessors(v, prev);

            Node* nxt=prev[0]->next[0];
            if(nxt&&!cmp_(v,nxt->value)){
                return false;
            }

            int lvl=randomLevel();
            if (lvl>level_){
                level_=lvl;
            }
            Node* node=new Node(v,lvl);
            for(int i=0;i<lvl;++i){
                node->next[i]=prev[i]->next[i];
                prev[i]->next[i]=node;
            }
            ++size_;
            return true;
        }

        bool erase(const T& v){
            std::vector<Node*> prev(kMaxLevel,head_.get());
            findPredecessors(v, prev);

            Node* target=prev[0]->next[0];
            if(!target||cmp_(v,target->value)){
                return false;
            }

            for(std::size_t i=0;i<target->next.size();++i){
                prev[i]->next[i]=target->next[i];
            }

            delete target;
            --size_;

            while(level_>1&&head_->next[level_-1]==nullptr){
                --level_;
            }
            return true;
        }

        void clear(){
            Node* cur=head_->next[0];
            while(cur){
                Node* nxt=cur->next[0];
                delete cur;
                cur=nxt;
            }
            std::fill(head_->next.begin(),head_->next.end(),nullptr);
            level_=1;
            size_=0;
        }

        std::vector<T> toVector() const {
            std::vector<T> res;
            res.reserve(size_);
            for(Node* cur=head_->next[0];cur;cur=cur->next[0]){
                res.push_back(cur->value);
            }
            return res;
        }

        void print() const {
            for(int i=level_ - 1;i>=0;i--){
                std::cout<<"L"<<i<<": ";
                for(Node* cur=head_->next[i];cur;cur=cur->next[i]){
                    std::cout<<cur->value<<" ";
                }
                std::cout << "\n";
            }
        }
};

void testSkipList(){
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

    SkipList<int> demo;
    for(int x:{5,3,8,1,9,2,7}){
        demo.insert(x);
    }
    std::cout<<"\n跳表示例结构：\n";
    demo.print();
}

int main(){
    testSkipList();
    return 0;
}
