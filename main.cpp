#include <iostream>
#include <eigen3/Eigen/Eigen>

using std::cout;
using std::endl;

int main() {
    double thres = 2.0;
    std::vector<Eigen::Vector3d> vPts;
    for (int i = 0; i < 10; ++i) {
        vPts.emplace_back(i, i, 0);
    }

    for (int i = 20; i < 30; ++i) {
        vPts.emplace_back(i, i, 0);
    }

    for (int i = 50; i < 60; ++i) {
        vPts.emplace_back(i, i, 0);
    }

    int numPts = vPts.size();

    std::random_shuffle(vPts.begin(), vPts.end());
    for (int i = 0; i < numPts; ++i) {
        cout << i << ": [" << vPts[i].transpose() << "]" << endl;
    }

    std::vector<bool> vbProcessed(numPts, false);  // 每个元素存储对应索引的点是否已经被处理过
    std::vector<std::vector<int> > vClusters;

    for (int i = 0; i < numPts; ++i) {
        if (vbProcessed[i]) {  // 这个点已经被加入到某个cluster中了，也就是前面的某个点长出来的cluster已经把它包进去了，它就不用再自立门户了
            // 或者理解为他已经在某个门派里了，不能再反出门派自己成立新的门派
            continue;
        }

        Eigen::Vector3d q = vPts[i];  // q本身不属于任何门派，所以可以自立山头
        std::vector<int> Q;  // 自立门户，创建一个cluster Q
        int slow_idx = 0;
        Q.push_back(i);
        vbProcessed[i] = true;  // 每当一个点被push_back到cluster里面，它就被判定为已经处理过了

        // 从头至尾遍历这个vector，有新元素加进来的话就往后push_back
        while (slow_idx < Q.size())  // 快慢指针，当慢指针追上快指针时，说明快指针已经停下了
        {
            std::vector<int> P;  // 该序列存储vPts中所有与点q的euclidean距离在thres以内的点
            for (int j = 0; j < numPts; ++j) {
                if (i == j) {  // // 自己还招自己啊，吃饱了撑的，也不用push_back进来，否则死循环了
                    continue;
                }

                // 这个点已经有了门派，不是江湖散人了，不能招揽入伙了，你能招揽的只有那些还在晃悠的，没被任何人招进门的
                if (vbProcessed[j]) {
                    // 如果这个点p已经被处理过了，也就是它已经被归到某个cluster里面了，那么有两种可能：
                    // 1. p就是在以Q为种子点的cluster里，那么肯定就不能重复push_back了
                    // 2. p不在以Q为种子点的cluster里，那么p跟q的距离肯定大于thres，否则在构建p_cluster时肯定把q给包进来了
                    continue;
                }

                // 这个点还没被其他门派或本门看上，可以试试笼络下，条件合适的话就收入本门
                if ((vPts[Q[slow_idx]] - vPts[j]).norm() < thres) {
                    Q.push_back(j);
                    vbProcessed[j] = true; // 条件合适，收入本门，同时对外宣称这个人我已经招了，不是闲云野鹤了
                }
            }

            slow_idx++;
        }

        vClusters.push_back(Q);
    }

    for (const auto &cluster : vClusters) {
        for (int id : cluster) {
            cout << id << " ";
        }
        cout << endl;
    }

    return 0;
}