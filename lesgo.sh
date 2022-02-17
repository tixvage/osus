echo "Github Automation";
echo "pls commit message";
read var1;
git pull origin master;
git add .;
git commit -m "$var1";
git push origin master;
echo "ins olmustur ne diim";
