#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 1600 // 大数组A的大小
#define PAGE_SIZE 16 // 页大小
#define NUM_PAGES (ARRAY_SIZE / PAGE_SIZE) // 总页数
#define NUM_FRAMES 10 // 小数组的个数，即页框数

int largeArray[ARRAY_SIZE]; // 大数组A
int frameB[PAGE_SIZE]; // 小数组B
int frameC[PAGE_SIZE]; // 小数组C
int frameD[PAGE_SIZE]; // 小数组D

int pageTable[NUM_PAGES]; // 页表，描述大数组的页与小数组序号的关系

int pageFaultCount = 0; // 缺页计数

// 初始化大数组A和小数组B、C、D
void initializeArrays() {
    srand(time(0)); // 随机数种子

    for (int i = 0; i < ARRAY_SIZE; i++) {
        largeArray[i] = rand()%1000; // 随机生成数组元素的值
    }

    for (int i = 0; i < PAGE_SIZE; i++) {
        frameB[i] = largeArray[i];
        frameC[i] = largeArray[PAGE_SIZE + i];
        frameD[i] = largeArray[2 * PAGE_SIZE + i];
    }
}

// 获取页号
int getPageNumber(int index) {
    return index / PAGE_SIZE;
}

// 模拟指令执行
void simulateInstructions() {
    for (int i = 0; i < ARRAY_SIZE; i++) {
        int pageNumber = getPageNumber(i); // 获取当前指令对应的页号

        // 访问小数组B、C、D，模拟页面置换
        if (pageTable[pageNumber] == 0) {
            pageFaultCount++;
            pageTable[pageNumber] = 1;

            // 根据页面淘汰算法进行页面置换
            // 这里使用FIFO算法，即将最早加入的页面置换出去
            int oldestPageNumber =0;
        for (int j = 0; j < NUM_PAGES; j++) {
            if (pageTable[j] < pageTable[oldestPageNumber]) {
                oldestPageNumber = j;
            }
        }
        // 将最早加入的页面置换出去，并将当前页面加入小数组B
        pageTable[oldestPageNumber] = 0;
        pageTable[pageNumber] = 1;

        for (int j = 0; j < PAGE_SIZE; j++) {
            frameB[j] = largeArray[pageNumber * PAGE_SIZE + j];
        }
    }

    // 打印访问到的数组元素
    printf("第%d次访问的元素: %d\n", i, largeArray[i]);
    }
}

int main() {
initializeArrays();
simulateInstructions();
// 打印缺页数
printf("缺页数: %d\n", pageFaultCount);
printf("缺页率: %.2f%%\n", (float)pageFaultCount/ARRAY_SIZE*100);

return 0;
}
