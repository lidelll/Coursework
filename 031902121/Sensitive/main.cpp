#include<iostream>
#include<fstream>
#include<sstream>
#include<list>
#include<cstring>
#include<stdio.h>
using namespace std;
class sw //定义敏感词类
{
public:
    string word="\0";//敏感词内容
    int len ;//敏感词的长度
    int f ;//判断是否是中文，f等于1则是中文
    int n;//敏感词号
}senw[100];

int sw_sum;//敏感词总数

class line
{
public:
    int len = 0;//该行长度
    string str = "\0";//该行内容
    int n;//行号
}lin[1000];

int line_sum;//总共的行数

class ans
{
public:
    int line_num;//行号
    int sw_num;//敏感词号数
    int ans_s;//答案在行中起始位置
    int ans_e;//答案在行中末尾位置
}an[5000];
int ans_sum = 0;//答案总数

void find(sw& w, line& lin)//定义search函数进行遍历匹配
{
    int i = 0, j = 0, k=0;
    if(w.f)//如果敏感词为中文的分支
    for (i = 0;i < lin.len-2;i++)
    {
        if (w.word[j] == lin.str[i] && w.word[j + 1] == lin.str[i + 1] && w.word[j + 2] == lin.str[i + 2])//用三字节utf-8编码匹配汉字
        {
            //如果插入数字或字母则跳过
            if (lin.str[i + 3] < 58 && lin.str[i + 3]>47 || lin.str[i + 3] > 64 && lin.str[i + 3] < 91 || lin.str[i + 3]>96 && lin.str[i + 3] < 123)continue;
            if (!j)k = i;
            i = i + 2;
            j = j + 3;

            if (j == w.len)//查找完成
            {
                an[ans_sum].ans_s = k;
                an[ans_sum].ans_e = i;
                an[ans_sum].line_num = lin.n;
                an[ans_sum++].sw_num = w.n;
                j = 0;      //从当前位置继续查找
            }
        }
    }
    else//敏感词为英文
    {
        for (i = 0;i < lin.len;i++)
            if ((w.word[j] == lin.str[i]) || (w.word[j] - lin.str[i] == 32 && lin.str[i] >= 'A') || (lin.str[i] - w.word[j] == 32 && lin.str[i] <= 'z'))//匹配成功
            {
                //如果插入不是敏感词的字母则跳过
                if (lin.str[i + 1] > 64 && lin.str[i + 1] < 91 || lin.str[i + 1]>96 && lin.str[i + 1] < 123)
                    if (!((w.word[j+1] == lin.str[i+1]) || (w.word[j+1] - lin.str[i+1] == 32 && lin.str[i+1] >= 'A') || (lin.str[i+1] - w.word[j+1] == 32 && lin.str[i+1] <= 'z')))continue;
                if (!j)k = i;//第一次匹配成功记录当前文本起始位置
                j++;

                if (j == w.len)
                {
                    an[ans_sum].ans_s = k;
                    an[ans_sum].ans_e = i;
                    an[ans_sum].line_num = lin.n;
                    an[ans_sum++].sw_num = w.n;
                    j = 0;
                }
            }
    }
}

int  main()
{
    system("chcp 65001");//将控制台设置为utf-8编码
    //定义文件输入输出流对象
    ifstream org;
    ifstream words;
    ofstream answer;
    string filename;//暂时存放文件路径与名称
    string s;
    //输入有效文件路径名并打开
    cin >> filename;
        words.open(filename);
    cin >> filename;
        org.open(filename);
    cin >> filename;
        answer.open(filename);
    if (!words)cout << "words open error!" << endl;
    if (!org)cout << "org open error!" << endl;
    if (!answer)cout << "test open error!" << endl;
    int i = 0, j = 0;
    //将敏感词存入敏感词类数组中并分类
    while (getline(words, s))
    {
        senw[i].word = s;
        senw[i].n = i;
        senw[i].len = s.length();
        senw[i++].f = senw[i].word[0] > 0 ? 0 : 1;
    }
    sw_sum = i;
    i = 0;
    //逐行输入测试文本并存入line类数组
    while (getline(org, s))
    {
        lin[i].len = s.length();
        lin[i].n = i;
        lin[i++].str = s;
    }
    line_sum = i;
    //用find函数查找敏感词
    for (i = 0;i < line_sum;i++)
        for (j = 0;j < sw_sum;j++)find(senw[j], lin[i]);
    //输出答案到选定的文件里
    answer<< "Total: " << ans_sum << endl;
    for (i = 0;i < ans_sum;i++)
    {
        answer << "Line" << an[i].line_num + 1 << ": <" << senw[an[i].sw_num].word << "> ";
        for (j = an[i].ans_s;j <= an[i].ans_e;j++)answer << lin[an[i].line_num].str[j];
        answer << endl;
    }
    //关闭文件
    org.close();
    words.close();
    answer.close();
    return 0;
}
