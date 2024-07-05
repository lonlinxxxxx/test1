#include<stdio.h>
#include<stdbool.h>
#include <stdlib.h>
#include<time.h>
/*
    昨天的思路补充完整是这样的：
    可完全取出的数组由连续数 123 234...和三同数 111 222 333...组成
    对一组数组的数字进行统计，每个数字的数量等于这个数字的三同数和相关的连续数
    用s[i]表示数字总数，a[i]表示以i为起点的连续数数量，b[i]表示这个数的三同数数量
    a[i]的范围是[0,2],因为如果a[i]大于等于3，就可以继续拆分到b[i]中 
    举例说明a[1] = 3,就会有123 123 123，但这同样可以拆分成111 222 333

    可以写出等式：s[i] = a[i-2] + a[i-1] + a[i] + 3b[i]
    特别的对边界1，因为没有1之前的连续数，所以有 s[1] = a[1] + 3b[1]，可得a[1] = s[1]%3
    继续推导 s[2] = a[1] + a[2] + 3b[2], 可得a[2] = (s[2]-a[1])%3
            s[3] = a[1] + a[2] + a[3] + 3b[3]，可得a[3] = (s[3]-a[1]-a[2])%3
            ...
            s[7] = a[5] + a[6] + a[7] + 3b[7]，可得a[7] = (s[7]-a[6]-a[5])%3
            s[8] = a[6] + a[7] + 3b[8],8以后又遇到了新的边界条件，没有新的连续数，也就没有新未知量
                                        (s[8] - a[6] -a[7])%3 = 0,可以用来判断整个数组的正确性
            s[9] = a[7] + 3b[9]，同上，(s[9] -a[7])%3 = 0,也是用来判断整个数组是否正确的条件 
    因此总结这个方法为，通过循环计算出a[6] a[7],判断s[8] s[9]的等式是否成立
    如果成立，就是可完全取出；反之则不能完全取出
    这个方法没有办法处理有0的情况，只有暴力解，就是把0变成1-9进行循环，寻找是否有完整取出的可能
*/
bool IfThree(int *l, int len){
    if(len%3 != 0)
        return false;
    int nums[10] = {0};//数字直接等于下标更直观，同时为后续添入0的方案做准备
    int ret[10] = {0};
    for(int i = 0; i < len; i++){
        nums[l[i]]++;//统计出s[i]
    }
    ret[1] = nums[1]%3;//计算a[1]
    for(int i = 0; i < 10; i++){
        printf("%d ",nums[i]);
        if(i > 1 && i < 8){
            ret[i] = (nums[i] - ret[i-1] -(i > 2?ret[i-2]:0))%3;//计算出a[2]-a[7]
        }
        printf("%d \n",ret[i]);
        if(i == 9){
            if((nums[8] - ret[6] -ret[7])%3 != 0 //判断s[8] s[9]的等式是否成立
            || (nums[9] -ret[7])%3 != 0)
                return false;//不成立就返回false
        }
    }
    return true;
}

/*    新思路：
        利用a[i]<= 2的特性，有a[1]<=2, a[1] <= a[1] + a[2]<= a[1] + 2 <=4, a[1]+a[2]+a[3]<=6
        每个数字数量尽量去除三同数，保留到连续数可能出现的最大值
        然后去除连续数，然后同时对三个位置进行判断，如果 a[1] != 0 && a[2] != 0 && a[3] != 0,a[1]a[2]a[3]同时减1
        循环整个数组，将连续数完全消除，此时如果还有数字不为0，或不为3，说明不能完全取尽

        引入0的癞子用法，当发现有数剩下且不为0或3时，对0的数量进行检查，如果有0，尝试进行补位
        先考虑2，如果数字为2，0数量大于0；补位消去2，0数量减一
        再考虑1，如果前一个位置或间隔一个位置值不为0，缺一个1组成连续数，0补位，消去连续数
        如果1附近没有其他值，0数量大于等于2，补成三同数，0数量减二

        遍历下来如果0数量用尽依然存在不为0或3的数，报错
*/
bool IfThreeNew(int *l, int len){
    if(len%3 != 0)
        return false;
    int nums[10] = {0};//数字直接等于下标更直观，同时为后续添入0的方案做准备
    int ret[10] = {0};//每个数上连续数的总数量
    int a[10] = {0};//由这个数开头的连续数数量 
    for(int i = 0; i < len; i++){
        nums[l[i]]++;//统计出s[i]
    }
    
    for(int i = 0; i < 10; i++){
        printf("\n%d ",nums[i]);
        ret[i] = nums[i]%3;
        if(i == 2){//限制条件 ret[1] = a[1] <=2, a[2] = ret[2] - ret[1] <=2
            if(ret[2] < ret[1])
                ret[i] += 3;
            a[1] = ret[1];
            a[2] = ret[2] - ret[1];
        }
            
        while (i > 2 && i< 8 && ret[i] - ret[i-1] + a[i-3]  < 0){//限制条件 0 <= a[i] <= 2
           ret[i] += 3;
           a[i] = ret[i] - ret[i-1] + a[i-3];
        }

        if(i == 9 && ret[8] < ret[9]){//限制条件 ret[9] = a[7] <=2, ret[8] = a[6] + a[7] >=a[9]
            ret[8] += 3;
        }
        printf("%d ",ret[i]);//去除三同数，使剩下的数值在连续数的最大值以下
    }

    for(int i = 1;i < 10; i++){
        while(i < 8&&ret[i] != 0 && ret[i+1] != 0 && ret[i+2] != 0){
            ret[i]--;
            ret[i+1]--;
            ret[i+2]--;
        }
        printf("\n%d",ret[i]);//去除连续数
    }

    for(int i = 1;i < 10; i++){
        if(nums[0] > 0){//如果存在0补位
            if(ret[i] == 2){//如果余数为2需要一个0补位
                ret[i] = 0;
                nums[0]--;
            }
            else if(ret[i] == 1){//如果余数为1，连续数缺一,补一个0
                if(i < 8 ){
                    if(ret[i+1] > 0){
                        ret[i] = 0;
                        ret[i+1]--;
                        nums[0]--;
                    }else if(ret[i+2] > 0){
                        ret[i] = 0;
                        ret[i+2]--;
                        nums[0]--;
                    }
                }else if(i == 8 && ret[i+1] > 0){
                    ret[i] = 0;
                    ret[i+1]--;
                    nums[0]--;
                }
                else if(nums[0] >=2 ){//如果余数为1且孤立，补两个0
                    ret[i] = 0;
                    nums[0] -= 2;
                }else{
                    return false;//0数量不够就报错
                }
            }
        }
        else if(ret[i] != 0 && ret[i] != 3)//没有0补位时，检测到不为0或3的数，直接报错
            return false;
    }
    return true;
}


/*
    添加随机数的连续数或三同数，并记录长度
*/
void RandTrue(int *l, int *size){
    srand(time(0));
    int randnum = rand()%10;
    for(int i = 1;i < 10;i++){
        randnum = rand()%10;
        *size += 3*randnum;
        for(int j = 0;j < 3*randnum; j++)//添加随机数数量的三同数
            *l++ = i;
        
        if(i<8){
            do{randnum = rand()%10;}
            while (randnum%3 == 0);
            *size += 3*randnum;
            for(int j = 0;j < 3*randnum; j++)//为7以前添加随机数数量的连续数
                *l++ = i+(j%3);
        }
    }
}

/*
    把数组内数字随机改写为0，因为随机数可能重复，最后得到的0数量可能比num小
    如果要生成不重复的随机数可以用数组记录然后遍历去判断是否重复
*/
void RandZero(int *l, int len, int num){
    srand(time(0));
    int randnum = rand()%len;
    int temp = 0;
    printf("\nrandomZero  ");
    while (num--)
    {
        while(temp == randnum)
            randnum = rand()%len;
        printf("%d ", randnum);
        *(l+randnum) = 0;
        temp = randnum;
    }
}

/*
    破坏完全取出的数组可以增加不平衡的项
    这里用把数组内数字随机改写破坏平衡，采用（原数+1）%10,同时避免出现0
*/
void RandFalse(int *l, int len, int num){
    srand(time(0)+100);
    int randnum = rand()%len;
    int temp = 0;
    printf("\nrandomFalse  ");
    while(num--){
        while(temp == randnum || *(l+randnum) == 9)
        randnum = rand()%len;
        printf("%d ", randnum);
        *(l+randnum) = (*(l + randnum) + 1)%10;
        temp = randnum;
    }
}

int main(){
    int nums[1000] = {};
    int len;
    RandTrue(nums, &len);//给数组写入正确可完全取出的数字
    RandFalse(nums, len, 1);//随机改写破坏完全可取出
    RandZero(nums, len, 1);//随机插入0
    if(IfThreeNew(nums, len))
        printf("\nlen %d success",len);
    else
        printf("\nlen %d fail",len); 

    return 1;
}