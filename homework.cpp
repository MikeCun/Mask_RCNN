/*
 * Created by Mike Cun in Feb 10
 * Using the Hash_table to find the maximum immediately
 * Using stack, vector and unordered_map from C++ STL
 */

#include <iostream>
#include <stack>
#include <vector>
#include <cstdio>
#include <unordered_map>

using namespace std;

/**
 * 用于本题目不合适输入检测以及错误提示
 * @param x
 * @param ruler: 你想要告诉用户的合法值范围
 * @param special_num: 特殊合法值（-1）
 * @return 判断用户是否输入合法
 */
bool error_input_msg (int x, bool special_num, string ruler) {
    if (special_num && x == -1) {
        return true;
    } else if (x <= 0 || x > 10000) {
        cout << "输入不合法！" << endl;
        cout << "请重新输入（" << ruler <<  "）： ";
        return false;
    }
    return true;
}

struct container {          //集装箱结构体，后续可增加其他参数
    int weight = 0;         //集装箱重量
    int stack_index = -1;   //集装箱在场地栈内位置
};

/**
* 输出集装箱的信息，包括重量和栈内位置
* @param m_container
* @return 集装箱信息
*/
string container_msg (container m_container) {
    return ("重量：" + to_string(m_container.weight) + "，栈内位置：" + to_string(m_container.stack_index) + "。");
}

/**
* 用于检测每行输入的结束标志若为Enter时就表示此行输入结束
* @param enter_break: 用户输入的下一个字符
* @return 判断用户是否输入了Enter
*/
bool enter_break_switch (char enter_break = (char)getchar()) {
    return (enter_break == '\n');
}

class Ground {                              //码头场地类Gound
private:
    int m_capacity;                         //场地容量
    int m_max_weight;                       //最大重量记录
    vector<container> m_max_weight_container_series;  //最重重量标记序列
    stack<container> m_ground_place;        //场地栈
    unordered_map<int, container> m_ground_map;   //场地集装箱重量与在栈内位置的映射，哈希表

public:
    Ground() {                              //构造函数，初始化类内变量
        m_capacity = 0;
        m_max_weight = 0;
    }
    /**
     * 设（设置堆场容量，即设置能容纳的最大集装箱数）
     * 设置了防止负数和零出现的情况
     * @param capacity
     * @return true: 能够正常设置场地容量
     */
    bool set_capacity(int capacity) {
        m_capacity = capacity;
        return true;
    }
    /**
     * 增（新集装箱入库）
     * @param my_container
     * @return true: 正常入库
     *         false: 场地已满，无法入库
     */
    bool push(container my_container) {
        if(m_ground_place.size() >= m_capacity) {
            //  若场地已满
            cout << container_msg(my_container) + "场地已满，无法再入库！" << endl;
            return false;
        }

        m_ground_place.push(my_container);
        //  将每个重量对应的栈内位置插入到Hash Table中
        int max_weight_index = (int)m_ground_place.size() - 1;
        int current_weight = my_container.weight;       //  记录当前集装箱的重量

        my_container.stack_index = max_weight_index;    //  记录此次的集装箱的栈内位置
        m_ground_map[current_weight] = my_container;    //  以集装箱重量为Key，Value为当前集装箱
        //  如果此次的集装箱重量大于在录的最大集装箱重量的记录，则更新
        if (current_weight > m_max_weight) {
            m_max_weight = current_weight;
            m_max_weight_container_series.clear();          //  更新之前相同重量的向量容器
            m_max_weight_container_series.push_back(my_container);  //  并进行重新记录
        } else if (current_weight == m_max_weight) {        //  若等于当前重量，则记录其下标位置
            m_max_weight_container_series.push_back(my_container);
        }
        cout << container_msg(my_container) << "入库成功！" << endl;
        return true;
    }
    /**
     * 删（最后入库的集装箱出库）
     * 设置了场地空的提示，同时也对于即将删除的最大重量的集装箱进行更新
     * @return true: 出库成功
     *         false: 场地已空，无法出库
     */
    bool pop() {
        if (m_ground_place.empty()) {
            //  若场地已空
            cout << "场地已空，无需出库！请您只能输入正数来进行入库：" << endl;
            return false;
        }
        container popped_container = m_ground_place.top();

        if (popped_container.weight == m_max_weight) {
            //  若最大重量的集装箱出库了，则删除其存在在最大向量内的记录
            m_max_weight_container_series.pop_back();
        }

        m_ground_place.pop();
        cout << container_msg(popped_container) << "出库成功！" << endl;
        return true;
    }
    /**
     * 查（查找仓库中是否有满足要求的集装箱，查找的条件为weight）
     * @param weight
     * @return 搜索到的同样重量的集装箱（最近入栈的）
     */
    container search_weight(int weight) {
        container same_weight_container = {};
        if (m_ground_place.empty()) {   //  场地已空
            cout << "场地已空，无法完成查询操作！" << endl;
        } else if (m_ground_map.find(weight) != m_ground_map.end()) {   //  在哈希表中查找到了
            same_weight_container = m_ground_map[weight];
        } else {    //  没有查找到
            cout << "没有重量为（ " << weight << " ）的集装箱！" << endl;
        }
        return same_weight_container;
    }
    /**
     * 检（给出当前库中重量最大的集装箱位置）
     * @return 最大重量集装箱序列向量
     */
    vector<container> find_max_weight() {
        if (m_max_weight_container_series.empty()) {    // 若没有找到，
            container no_container;
            no_container.weight = 0;
            no_container.stack_index = -1;
            m_max_weight_container_series.push_back(no_container);
        }

        return m_max_weight_container_series;
    }
    /**
     * 显（打印列出当前堆场中的所有集装箱重量）
     */
    void print_all() {
        stack<container> tempt_stack;  //  设置临时记录出栈的集装箱

        while (!m_ground_place.empty()) {
            cout << container_msg(m_ground_place.top()) << endl;
            tempt_stack.push(m_ground_place.top());
            m_ground_place.pop();
        }
        cout << "全部打印完成！" << endl;
        //  按照原来的位置装入集装箱
        while (!tempt_stack.empty()) {
            m_ground_place.push(tempt_stack.top());
            tempt_stack.pop();
        }
    }
};

int main() {
    int ground_capacity;            //堆场容量
    int ground_show;                //是否需要输出的标志
    int ground_weight;              //集装箱重量
    int ground_search_series;       //集装箱重量搜索的输入序列
    container ground_container;     //

    vector<int> ground_search;      //
    Ground Ground_Place;            //新建堆场

    cout << "请输入堆场的最大容量（大于0）：";
    while (cin >> ground_capacity) {    //  检查堆场容量是否合法，直至合法位置
        if (error_input_msg(ground_capacity, false, "输入 > 0")) {
            Ground_Place.set_capacity(ground_capacity);
        } else {    //  不合法再次重新输入
            continue;
        }

        if (enter_break_switch()) { //  Enter为此行输入结束标志，用来结束此循环
            break;
        }
    }

    cout << "入库与出库（正数代表入库，-1代表出库，以空格作为间隔，回车键表示出入库过程结束）：" << endl;
    while(cin >> ground_weight) {
        if (ground_weight == -1) {  //  出库
            if  (!Ground_Place.pop()) { //  若场地已空，则只能入库
                continue;
            }
        } else if (error_input_msg(ground_weight, false, "0 < 输入 < 10000")) {
            //  检测输入的重量的值是否合法，合法则入库，不合法直至输入到合法值为止
            ground_container.weight = ground_weight;
            Ground_Place.push(ground_container);
        } else {    //  不合法再次重新输入
            continue;
        }

        if (enter_break_switch()) { //  Enter为此行输入结束标志，用来结束此循环
            break;
        }
    }

    cout << "集装箱重量搜索结果（-1表示查找最大重量的集装箱，其余正数表示查找对应重量的集装箱）" << endl;
    cout << "以空格作为分隔，回车表示输入结束： ";
    //  先统一接收收入，然后在依次进行查询
    while (cin >> ground_search_series) {
        if (!error_input_msg(ground_search_series, true,"0 < 输入 < 10000")) {
            //  检测输入的重量的值是否合法，合法则记录，不合法直至输入到合法值为止
            continue;
        } else {    //  合法则进行记录
            ground_search.push_back(ground_search_series);
        }

        if (enter_break_switch()) { //  Enter为此行输入结束标志，用来结束此循环
            break;
        }
    }

    cout << "集装箱重量搜索结果：" << endl;
    for (auto iter = ground_search.begin(); iter != ground_search.end(); iter++) {
        if ((*iter) == -1) {
            vector<container> max_weight_series = Ground_Place.find_max_weight();
            cout << "以下是最大重量的集装箱的信息：" << endl;
            for (auto item = max_weight_series.begin(); item != max_weight_series.end(); item++) {
                cout << container_msg(*item) << endl;
            }
        } else {
            cout << "以下是相同重量的集装箱的信息：" << endl;
            container same_weight_container = Ground_Place.search_weight(*iter);
            cout << container_msg(same_weight_container) << endl;
        }
    }

    cout << "请问是否需要打印堆场内所有的集装箱信息（包括重量和栈内位置，其余整数值代表打印，0表示不打印）：" << endl;
    cin >> ground_show;
    if (ground_show) {
        Ground_Place.print_all();
    }

    return 0;
}
