labels=(airplane automobile bird cat deer dog frog horse ship truck)
ids=(1 2 3 4 5 6 7 8 9 10)
for label in ${labels[@]}
do
    for id in ${ids[@]}
    do
        img="${label}$id.png"
        if [ ! -e $img ] ; then
            img_link="http://www.cs.toronto.edu/~kriz/cifar-10-sample/$img"
            echo "Download $img from $img_link"
            wget $img_link
        fi
    done
done
