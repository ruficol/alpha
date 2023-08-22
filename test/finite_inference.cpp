#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

// 定义机器人类
class Robot {
public:
    int id;  // 机器人ID
    int lockedPoint;  // 当前锁定的待锁点
    unordered_set<int> lockedPointsList;  // 已锁定的点列表
};

// 推理函数
bool inference(vector<Robot>& robots, vector<vector<int>>& lockOrderMatrix, vector<int>& coordinateList, unordered_set<int>& blockingList, int k, int depth) {
    // 推理深度达到预定值，返回推理成功
    if (k == depth) {
        return true;
    }
    
    Robot& currentRobot = robots[k];  // 当前机器人
    int currentLockPoint = coordinateList[currentRobot.id];  // 当前机器人应上锁的待锁点
    
    for (int i = 0; i < robots.size(); i++) {
        Robot& otherRobot = robots[i];  // 其他机器人
        
        // 锁顺序矩阵中的对应元素为0，表示当前机器人和其他机器人存在干涉。currentRobot.id为行索引，otherRobot.id * depth计算出另一个机器人的标识符在矩阵中的偏移量
        if (lockOrderMatrix[currentRobot.id][otherRobot.id * depth + currentLockPoint] == 0) {
            
            // 如果当前机器人已经在二阶阻拦列表中，推理失败，返回false
            if (blockingList.find(currentRobot.id) != blockingList.end()) {
                return false;
            }
            
            // 将当前机器人添加到二阶阻拦列表中
            blockingList.insert(currentRobot.id);
            
            // 更新其他机器人的锁定点并递归地进行推理，递归调用inference函数进行进一步的推理
            coordinateList[otherRobot.id] = coordinateList[otherRobot.id] + 1;
            bool inferenceSuccess = inference(robots, lockOrderMatrix, coordinateList, blockingList, i, depth);
            
            if (inferenceSuccess) {
                // 如果推理成功，从二阶阻拦列表中移除当前机器人
                blockingList.erase(currentRobot.id);
            } else {
                // 推理失败，返回false
                return false;
            }
        }
    }
    
    // 设置当前机器人的锁定点，lockedPointsList用于存储当前机器人已锁定的点的集合
    currentRobot.lockedPoint = currentLockPoint;
    currentRobot.lockedPointsList.insert(currentLockPoint);
    
    // 递归调用进行下一个待锁点的推理
    return inference(robots, lockOrderMatrix, coordinateList, blockingList, k + 1, depth);
}

int main() {
    int numRobots = 3;  // 机器人数量
    int depth = 3;  // 推理深度
    
    // 创建机器人对象
    vector<Robot> robots;
    for (int i = 0; i < numRobots; i++) {
        robots.push_back({i, 0}); // 将机器人对象添加到robots向量中
    }
    
    // 创建锁顺序矩阵，用于存储锁顺序矩阵
    vector<vector<int>> lockOrderMatrix(numRobots, vector<int>(numRobots * depth));
    // 填充锁顺序矩阵
    
    // 创建机器人坐标列表，用于存储机器人的坐标
    vector<int> coordinateList(numRobots, 0);
    
    // 创建二阶阻拦列表
    unordered_set<int> blockingList;
    
    // 执行推理
    bool inferenceSuccess = inference(robots, lockOrderMatrix, coordinateList, blockingList, 0, depth);
    
    if (inferenceSuccess) {
        cout << "Inference success!" << endl;
    } else {
        cout << "Inference failed!" << endl;
    }
    
    return 0;
}