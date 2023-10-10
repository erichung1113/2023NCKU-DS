make

function test_case() {
    hwId=$1
    proId=$2
    caseId=$3
    test_dir=${PWD}/test/${hwId}_${proId}
    gtimeout 1 ${PWD}/output/${hwId}_${proId}.output < ${test_dir}/input_${caseId}.txt >> ${PWD}/tmp_A.txt
    ret=$?
    if [ $ret -ne 0 ]; then
        echo "${hwId} ${proId} input_${caseId}.txt RE/TLE"
    else
        cat "${test_dir}/output_${caseId}.txt" > ${PWD}/tmp_B.txt
        diff -w ${PWD}/tmp_A.txt ${PWD}/tmp_B.txt > /dev/null
        ret=$?

        if [ $ret -eq 0 ]; then 
            echo "${hwId} ${proId} input_${caseId}.txt AC"
        else
            echo "${hwId} ${proId} input_${caseId}.txt WA"
        fi
    fi
    rm -f ${PWD}/tmp_A.txt
    rm -f ${PWD}/tmp_B.txt
    return $ret
}

function test_all() {
    for dirname in $(ls ${PWD}/test)
    do
        hwId=$(echo $dirname | awk -F '_' '{print $1}')
        proId=$(echo $dirname | awk -F '_' '{print $2}')
        for filename in $(ls ${PWD}/test/${dirname})
        do
            caseId=$(echo $filename | awk -F 'input_|.txt' '{print $2}')
            if [ -n "$caseId" ]; then
                test_case $hwId $proId $caseId
            fi
        done
    done
    return 0
}

[ ! -d "${PWD}/test" ] && tar zxf ${PWD}/test.tar.gz

if [ -n "$1" ] && [ -n "$2" ] && [ -n "$3" ]; then
    test_case $1 $2 $3
else
    test_all
fi
