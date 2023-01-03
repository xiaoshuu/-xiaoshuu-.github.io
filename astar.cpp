//
// Created by xiaoshuu on 23-1-2.
//
#include <iostream>
#include <cstring>
#include <algorithm>
#include <queue>


using namespace std;

const int N=15;

int start_x,start_y;
int end_x,end_y;

int map[N][N]={{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
               {1,0,0,0,0,0,0,0,1,0,0,0,0,0,1},
               {1,0,0,0,1,1,1,0,1,0,0,0,0,0,1},
               {1,0,0,0,1,0,0,0,1,0,1,1,1,0,1},
               {1,0,0,0,1,0,0,0,1,0,0,1,0,0,1},
               {1,0,1,1,1,0,1,0,1,0,0,1,0,0,1},
               {1,0,0,0,1,0,1,0,1,0,0,1,0,0,1},
               {1,1,1,0,1,0,1,0,0,0,0,1,0,0,1},
               {1,0,0,0,1,0,0,1,1,1,1,1,0,0,1},
               {1,0,0,0,1,0,0,0,0,1,0,0,0,0,1},
               {1,0,1,1,1,1,1,0,0,1,0,1,1,1,1},
               {1,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
               {1,1,1,1,1,1,1,0,0,1,1,1,1,0,1},
               {1,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
               {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
               };

bool is_valid(int x,int y){
    return x>=0&&x<N&&y>=0&&y<N&&map[x][y]==0;
}

struct Node{
    int x,y;//坐标
    int f,g,h;//代价函数值  g为起点到当前状态的实际代价 即起点到当前状态经过的边数 h为当前状态到终点的估计代价
    Node(int x=-1,int y=-1,int g=-1,int h=-1):x(x),y(y),g(g),h(h){
        f=g+h;
    }
};

Node parent[N][N];

//使用优先队列维护开放列表
struct Cmp{
    bool operator()(Node &a,Node &b){
        return a.f>b.f;
    }
};
priority_queue<Node,vector<Node>,Cmp>open_list;
/*
 * 此处穿插优先队列讲解：https://huangdy.blog.csdn.net/article/details/98626474?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-98626474-blog-117827166.pc_relevant_recovery_v2&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-98626474-blog-117827166.pc_relevant_recovery_v2&utm_relevant_index=2
 */


bool in_open_list[N][N];
bool in_close_list[N][N];

void get_path(Node cur){
    if(cur.x==start_x&&cur.y==start_y){
        return;
    }
    cout<<"("<<cur.y<<","<<cur.x<<")->";
    //递归查找父状态
    get_path(parent[cur.x][cur.y]);
}

void a_star(){
    memset(in_open_list,0,sizeof(in_open_list));
    memset(in_close_list,0, sizeof(in_close_list));
    /*
     * 此处穿插memset函数相关与其与fill的区别
     * https://blog.csdn.net/weixin_44162361/article/details/115790452?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522167270757616782425114617%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=167270757616782425114617&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-1-115790452-null-null.142^v68^control,201^v4^add_ask,213^v2^t3_esquery_v1&utm_term=memset&spm=1018.2226.3001.4187
     *https://blog.csdn.net/qq_42835910/article/details/86668951?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522167272774016800215039090%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=167272774016800215039090&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduend~default-1-86668951-null-null.142^v68^control,201^v4^add_ask,213^v2^t3_esquery_v1&utm_term=memset%E5%92%8Cfill&spm=1018.2226.3001.4187
     */

    //初始化起点
    Node start(start_x,start_y,0,abs(end_x-start_x)+abs(end_y-start_y));//abs为取绝对值
    open_list.push(start);
    in_open_list[start_x][start_y]=true;

    while(!open_list.empty()){
        //从开放列表中取出f值最小的状态
        Node cur=open_list.top();
        open_list.pop();
        in_open_list[cur.x][cur.y]=false;
        in_close_list[cur.x][cur.y]=true;
        if(cur.x==end_x&&cur.y==end_y){
            cout<<"Find a path!"<<endl;
            cout<<"The path is: ";
            get_path(cur);
            cout<<"("<<start_y<<","<<start_x<<")"<<endl;
            return;
        }
        for(int dx=-1;dx<=1;dx++){//枚举当前状态的每一个相邻状态
            for(int dy=-1;dy<=1;dy++){
                if(dx==0&&dy==0)continue;//跳过自身
                int x=cur.x+dx;
                int y=cur.y+dy;
                if(!is_valid(x,y))continue;
                if(in_close_list[x][y])continue;
                int g=cur.g+1;
                int h=abs(end_x-x)+abs(end_y-y);
                Node neighbor(x,y,g,h);
                if(!in_open_list[x][y]){
                    open_list.push(neighbor);
                    in_open_list[x][y]=true;
                    parent[x][y]=cur;
                }else if(g<cur.g){
                    parent[x][y]=cur;
                    neighbor.f=g+h;
                    open_list.push(neighbor);
                }
            }
        }
    }
    cout<<"Can't find a path!"<<endl;
}

int main(){
    cin>>start_y>>start_x;
    cin>>end_y>>end_x;
    a_star();
    return 0;
}
