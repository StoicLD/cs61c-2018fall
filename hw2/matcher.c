/**
 * As in every CS skeleton, the contents of this file are
 * only a suggestion. Please feel free to change anything.
 */

//
//  main.c
//  newrgrep
//
//  Created by 729790146 on 18/9/6.
//  Copyright © 2018年 729790146. All rights reserved.
//

#include <stdio.h>
int sub_match(char *pattern,char *text,int no_backslash);
/*int normal_match(char *pattern,char *text)
{
    if(*text=='\0')
        return 1;
    if(*text!='\0'&&(pattern[0]=='.'||pattern[0]==*text))
        return sub_match(pattern+1, text+1,no_backslash);
    return 0;
}*/

int matchplus(char c,char *pattern,char *text,int no_backslash)      //匹配加号，一个或者多个的情况
{
    while(*text!='\0'&&(*text++==c||c=='.'))
    {
        if(sub_match(pattern, text, no_backslash^1))
            return 1;
    }
    return 0;
}

/*
int matchstar(char c,char *pattern,char *text)       //匹配任意多个字符
{
    do{
        if(sub_match(pattern, text,))
            return 1;
    }while (*text!='\0'&&(*text++==c||c=='.'));
    return 0;
}
*/
int sub_match(char *pattern,char *text,int no_backslash)
{
    //解释如下：每次进入子函数都表明消耗掉当前一个token（一个匹配单元），说明已经text的一个字符
    //和pattern的一个token已经匹配。一直递归下去，直到出现return 0，表示匹配失败,否则整个pattern
    //都被消耗完了，说明匹配成功了，因此有下面的第一个if语句返回1
    if(no_backslash && pattern[0]=='\0')
        return 1;
    
    if(no_backslash && pattern[0]=='\\')        //转义字符直接跳过到下个处理,目前情况是假定输入合法
        return sub_match(pattern+1, text, 0);
    
    if(no_backslash && pattern[1]=='?')     //token结束
        return sub_match(pattern+2, text, no_backslash|1);
    
    if(*text!='\0'&&(pattern[0]=='.'||pattern[0]==*text))
    {
        if(pattern[1]=='+')
            return matchplus(pattern[0], pattern+2, text,no_backslash);
        return sub_match(pattern+1, text+1, no_backslash|1);
    }
    return 0;
}
int rgrep_matches(char *line, char *pattern) {
 	    while (*line!='\0') {
        //针对每个从*text++下标开始的字字符串进行匹配
        if(sub_match(pattern,line,1))
            return 1;
        line++;
    }
    return 0;   
}
