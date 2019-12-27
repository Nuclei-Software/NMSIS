#!/bin/sh

REPO_NAME="github.com/Nuclei-Software/NMSIS.git"
REPO_LINK="https://""${GH_TOKEN}""@""${REPO_NAME}"

die()
{
    echo " *** ERROR: " $*
    exit 1
}

#set -x
# Install Tools
sudo apt-get update || die
sudo apt-get install lib32z1 || die
sudo apt-get install dos2unix || die
sudo apt-get install doxygen || die

pip install --upgrade pip
pip install -r NMSIS/doc/requirements.txt

# Build documentation
echo 'Generating documentation ...'
cd NMSIS/doc
make all || die "Failed to build sphinx documentation"

# Check if this is a pull request
if [ "$TRAVIS_PULL_REQUEST" != "false" ] ; then
    echo "Don't push built docs to gh-pages for pull request "
    exit 0
fi

# Check if this is master branch
# Only push doc changes to gh-pages when this is master branch
if [ "$TRAVIS_BRANCH" != "master" ] ; then
    echo "Don't push built docs to gh-pages for non master branch "
    exit 0
fi

echo 'Push generated documentation to gh-pages branch...'

# Only commit changes when it is not a pull request
# tar doc
cd build
tar czf nmsis_doc.tar.gz html doxygen || die "Failed to achieve documentation"

git fetch origin || die
git branch -a || die
mkdir gh-pages || die
cd gh-pages || die
git init . || die
git remote add origin ${REPO_LINK} || die
git fetch origin -t || die
git checkout -b gh-pages origin/gh-pages || die
rm -rf doxygen html
cp ../nmsis_doc.tar.gz . || die
tar xzf nmsis_doc.tar.gz || die
rm -rf nmsis_doc.tar.gz || die

git add --all || die
git commit -s -a -m "doc: Push updated generated sphinx documentation of commit ${TRAVIS_COMMIT}" || die
if [ $? -eq 0 ] ; then
    echo 'Push changes to gh-pages branch.'
    git push ${REPO_LINK} gh-pages:gh-pages > /dev/null 2>&1 || die
else
    echo 'No update in gh-pages branch, no need to push changes!'
fi

exit 0
