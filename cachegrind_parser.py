import sys
import re

def fileReader(src_path):
    result=[]
    f=open(src_path,"r")
    for line in f:
        result.append(line)
    return result

def getSrcPath():
    try:
        src_path=sys.argv[1]
        minimum=int(sys.argv[2])
    except:
        print("usage : "+sys.argv[0]+" src_path minimum")
        exit()
    return src_path,minimum

def getFuncAndSourceLines(lines):
    func_lines=[]
    idx=0
    source_lines=[]

    for line in lines:
        temp=line.split(" ")
        try:
            if temp[1] != "Auto-annotated":
                func_lines.append(temp)
            else:
                idx=lines.index(line)
                break
        except:
            continue
    for i in range(idx,len(lines)):
        source_lines.append(lines[i])

    return func_lines,source_lines


def SourcePrint(func_data):
    print_array=[]
    for name,value in func_data.items():
        sen=[name,value]
        print_array.append(sen)
    print_array.sort(key=lambda x : int(x[1][0]),reverse=True)

    for sen in print_array:
        print("{: >50} {: >20}".format(*sen))
    print("------------------------------------------------------------------------------------------------------------------------------------------------------------------------")

def FuncPrint(func_data):
    print_array=[]
    print("------------------------------------------------------------------------------------------------------------------------------------------------------------------------")
    for name,value in func_data.items():
        sen=[name,str(value)]
        print_array.append(sen)
    print_array.sort(key=lambda x : int(x[1]),reverse=True)
    for sen in print_array:
        print("{: >60} {: >20}".format(*sen))
    print("------------------------------------------------------------------------------------------------------------------------------------------------------------------------")
    print "total access = "+str(totalAccess(func_data))
    print("------------------------------------------------------------------------------------------------------------------------------------------------------------------------")
    print()

def totalAccess(func_data):
    result=0
    for access in func_data.values():
        result+=access
    return result

def getManyAccessLines(source_lines,minimum):
    source={}
    temp_source=""
    for line in source_lines:
        line=re.sub(",","",line)
        temp=line.split()
        try:
            if temp[1] =="Auto-annotated":
                temp_source=temp[3]
        except:
            pass
        try:
            data_read=int(temp[3])
            data_write=int(temp[6])
            current_source=temp_source
            sum_=data_write+data_read
            if sum_ > minimum:
                sourceline= "".join(temp[9:])
                source[sourceline]=[sum_,current_source]
            else:
                continue
        except:
            continue
    return source

def getMemoryReadWriteAccess(func_lines):
    func={}
    for line in func_lines:
        data=' '.join(line)
        data=re.sub(",","",data)
        temp=data.split()
        try:
            data_read=int(temp[3])
            data_write=int(temp[6])
            func_name=temp[-1].split("/")[-1]
            if func_name.split(":")[0] == "???" or func_name.split(":")[0] == "TOTALS":
                continue
            func[func_name]=data_write+data_read
        except:
            continue
    return func

def processBeforeSourceCode(func_data):
    func_data=getMemoryReadWriteAccess(func_lines)
    FuncPrint(func_data)

def AfterProcess(source_lines,minimum):
    source_data=getManyAccessLines(source_lines,minimum) # dict -> { sourcecode : access,currentsource }
    SourcePrint(source_data)

if __name__=="__main__":

    src_path,minimum=getSrcPath()
    lines=fileReader(src_path)
    func_lines,source_lines=getFuncAndSourceLines(lines)

    processBeforeSourceCode(lines)
    AfterProcess(source_lines,minimum)
