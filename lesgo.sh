echo "Github Automation";
git pull origin master;
git add .;
git commit -m "$1";
git push origin master;
echo "ins olmustur ne diim";
