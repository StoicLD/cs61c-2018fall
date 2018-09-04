/**
 * As in every CS skeleton, the contents of this file are
 * only a suggestion. Please feel free to change anything.
 */
typedef int bool;
#define TRUE 1
#define FALSE 0

#include <string.h>
#include <stdio.h>

/**
 * Parses pattern for next character and sets
 * necessary parameters that are referenced inside/outside
 * of this function.
 *
 * Add any additional inputs to function signature if necessary.
 */
/**
 * Returns true if partial_line matches pattern, starting from
 * the first char of partial_line.
 */
int matches_leading(char *partial_line, char *pattern) {
    // You can use this recommended helper function, or not.
    return 0;
}

int special_char(char *c){
    int t=0;
    if(*c=='\\')
        t=1;
    else if(*c=='.')
        t=1;
    else if(*c=='+')
        t=1;
    else if(*c=='?')
        t=1;
    return t;
}
struct map
{
    char c;
    int value;
    bool continous;
};
/**
 * Implementation of your matcher function, which
 * will be called by the main program.
 *
 * You may assume that both line and pattern point
 * to reasonably short, null-terminated strings.
 *
 * This function is REQUIRED and is not a suggestion.
 */
int rgrep_matches(char *line, char *pattern) {
    
    //
    // TODO: put your code here.
    //
    int length =strlen(line);
    int plength=strlen(pattern);
    //BOOL型用来判断转义字符和多重字符等等
    bool is_match=FALSE;
    struct map previous={'a','0',FALSE};		//这个map用来记录前一个token中字符出现的次数
    for(int i=0,k=0,global=0;i<length;i++)
    {
        //global是全局字符比对的起始位置，字符串匹配的时候要有一个记录起始位置的变量！！！
        //想法：针对pattern，在循环内组装成一个子pattern，由这个子pattern进行匹配
        int head=k,back=k;
        if(head<plength && back<plength)
        {
            //针对pattern唯一
            back++;
            //(1)token只有一个符号的情况
            if(back>=plength)
            {
                //表示pattern已经到尾部了
                back--;		//back回退到与head重合
                if(pattern[head]==line[i]||pattern[head]=='.')
                    is_match=TRUE;
                else
                    is_match=FALSE;
            }
            //这里是pattern至少有两个符号的情况
            else
            {
                /*
                 head的情况分为三种
                 (1)head 处是普通字符或者. ,直接看back是否可以结合
                 (2)head处是转义字符,此时考虑整个token可能为3的情况，如'\++'
                 
                 back情况分为
                 (1)back是'+'
                 (2)back是'?'
                 (3)back是其他字符，不结合
                 */
                char now=pattern[head];		//当前的模式元字符(两元情况)
                //(2)输入不合法
                if(now=='+'||now=='?')
                {
                    //输入不合法的情况
                    printf("%s\n", "input is not valid!");
                    return 0;
                }
                
                //(3)开头是'\'，表明可能会有三个字符的token情况
                if(now=='\\')
                {
                    head++;
                    now=pattern[head];
                    if((back+1)<plength)
                    {
                        //这里考虑到 '\++' 这种情况，back不一定只比head大0或1
                        back++;
                        //不是特殊字符，那么back就只比head大一，组成一个token
                        
                        //(4)考虑三个字符组成的情况,?
                        //？的处理暂时是任意字符
                        if(pattern[back]=='?')
                        {
                            i--;
                            is_match=TRUE;
                        }
                        
                        //(5)考虑三个字符组成的情况,'+'的情况
                        else if(pattern[back]=='+')
                        {
                            if(line[i]==pattern[head]||now=='.')
                            {
                                is_match=TRUE;
                                previous.c=pattern[head];
                                previous.value=1;	//1表明可以一直消耗这个 'a+' token
                            }
                            else
                                is_match=FALSE;
                        }
                        //(6)不是三个字符的情况，只有一个有效字符如'\a','\.','\+'
                        else
                        {
                            back--;
                            if(line[i]==now||now=='.')
                                is_match=TRUE;
                            else
                                is_match=FALSE;
                        }
                    }
                    //(6)不是三个字符的情况，只有一个有效字符如'\a','\.','\+',且back走到pattern的末尾了
                    else
                    {
                        //不是？或者+的情况
                        if(line[i]==now||now=='.')
                            is_match=TRUE;
                        else
                            is_match=FALSE;
                    }
                        
                }
                
                //开头不是'\',至多只有两个符号组成token
                else
                {
                    
                    //(7)'+'号的情况，如'a+','.+'
                    if(pattern[back]=='+')
                    {
                        if((line[i]==pattern[head])||(pattern[head]=='.'))
                        {
                            is_match=TRUE;
                            previous.c=pattern[head];
                            previous.value=1;	//1表明可以一直消耗这个 'a+' token
                        }
                        else
                            is_match=FALSE;
                    }
                    
                    //(8)'?'号的情况，我的选择是直接跳过，并且line的下标回退一个（下个循环还是这个位置）
                    else if(pattern[back]=='?')
                    {
                        i--;
                        is_match=TRUE;
                    }
                    
                    //(9)最最普通的情况，也就是没有任何特殊字符，只有.或者普通字符
                    else
                    {
                        back--;			//back回退到与head重合
                        if((line[i]==pattern[head])||(pattern[head]=='.'))
                            is_match=TRUE;
                        else
                            is_match=FALSE;
                    }
                }
            }
            //token的组装结束，进入判定结尾环节
            
            
            //先判断是否token已经走完了,就是是否全部匹配，如果是直接返回1
            if(back==plength-1)
            {
                if(is_match==1)
                    return 1;
                else
                {
                    if(previous.value==1&&previous.c==line[i])
                    {
                        is_match=TRUE;      //这种情况下k不变
                        head--;
                    }
                    else
                    {
                        k=0;
                        i=global;
                        global++;
                    }
                }
            }
            //如果没走完。在确定当前token匹配结果前,如果前面一个token是a+形式的,那么当前token不匹配的时候可以使用前面的
            else
            {
                    if(is_match==0)
                    {
                        if(previous.value==1&&previous.c==line[i])
                        {
                            is_match=TRUE;      //这种情况下k也不变
                            head--;
                        }
                        else
                        {
                            k=0;        //从头开始的同时，i不能动！！！！
                            i=global;
                            global++;
                        }
                    }
                    else            //如果match了，那么要修改,同时k增加
                    {
                        previous.c=pattern[head];
                        if(pattern[back]=='+')
                            previous.value=1;
                        else
                            previous.value=0;
                        k=back+1;
                    }
            }
            //接着下一个token
        }
    }
    return 0;
}
