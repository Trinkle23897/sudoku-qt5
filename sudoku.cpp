#include <cstdio>
#include <ctime>
#include <algorithm>
const int pos[9][9]={{0,0,0,1,1,1,2,2,2},{0,0,0,1,1,1,2,2,2},{0,0,0,1,1,1,2,2,2},{3,3,3,4,4,4,5,5,5},{3,3,3,4,4,4,5,5,5},{3,3,3,4,4,4,5,5,5},{6,6,6,7,7,7,8,8,8},{6,6,6,7,7,7,8,8,8},{6,6,6,7,7,7,8,8,8}};
struct Mat{
	int m[9][9];
	int cnt0(){
		int cnt=0;
		for(int i=0;i<9;++i)
			for(int j=0;j<9;++j)
				if(m[i][j]==0)
					cnt++;
		return cnt;
	}
	int read(){
		char ch;
		for(int i=0;i<9;++i)
			for(int j=0;j<9;++j){
				while(ch=getchar(),!(ch>='0'&&ch<='9')&&ch!='_'&&ch!='.')
					if(ch<=0)return 0;
				if(ch=='_'||ch=='.')m[i][j]=0;
				else m[i][j]=ch-'0';
			}
		return 1;
	}
	void print(){
		for(int i=0;i<9;++i)
		{
			if(i==3||i==6)puts("------+-------+------");
			for(int j=0;j<9;++j)
			{
				printf("%c%c",m[i][j]==0?'_':m[i][j]+'0',j==8?'\n':' ');
				if(j==2||j==5)printf("| ");
			}
		}
		puts("");
	}
}zero;
struct Trip{int x,y,v;};
class Solver{
public:
	Mat a,showtime;
private:
	int s0[9],s1[9],s2[9],ans,tot,limit,idx[1024],cnt;
	Trip swp,p[100],q[100];Mat tmp_m;
	int calc(int x){
		int f=0;
		while(x)f+=x&1,x>>=1;
		return f;
	}
#define add(i,j,k,v) s0[i]^=v,s1[j]^=v,s2[k]^=v
	void dfs(int t){
		if(t==tot+1)
			ans++;//,a.print();
		else{
			int x=p[t].x,y=p[t].y,z=pos[x][y];
			for(int s3=s0[x]&s1[y]&s2[z],i;s3&&ans<limit;s3-=i)
				i=s3&-s3,add(x,y,z,i),a.m[x][y]=idx[i],dfs(t+1),add(x,y,z,i);
		}
	}
	int only1sol(int m,int symm)
	{
		for(int i=1;i<=m;++i)
		{
			tmp_m.m[q[i].x][q[i].y]=0;
			if(symm)
				tmp_m.m[8-q[i].x][8-q[i].y]=0;
		}
		int res=solve(tmp_m)==1;
		for(int i=1;i<=81;++i)
			tmp_m.m[q[i].x][q[i].y]=q[i].v;
		return res;
	}
	int generate(int symm)
	{
		solve(zero);
		cnt=0;
		for(int i=0;i<9;++i)
			for(int j=0;j<9;++j)
				q[++cnt]=(Trip){i,j,a.m[i][j]};
		if(symm)cnt=41;
		std::random_shuffle(q+1,q+1+cnt);
		tmp_m=a;int blank=0;
		for(int i=1,tmpa,tmpb;i<=cnt;++i)
		{
			tmpa=tmp_m.m[q[i].x][q[i].y];
			if(symm)tmpb=tmp_m.m[8-q[i].x][8-q[i].y];
			tmp_m.m[q[i].x][q[i].y]=0;
			if(symm)tmp_m.m[8-q[i].x][8-q[i].y]=0;
			if(solve(tmp_m)>1)
			{
				tmp_m.m[q[i].x][q[i].y]=tmpa;
				if(symm)tmp_m.m[8-q[i].x][8-q[i].y]=tmpb;
			}
			else blank++;
		}
		// //if(!only1sol(L,symm))return 0;
		// while(L<R)
		// {
		// 	int mid=L+R+1>>1;
		// 	if(only1sol(mid,symm))L=mid;
		// 	else R=mid-1;
		// }
		// for(int i=1;i<=L;++i)
		// {
		// 	tmp_m.m[q[i].x][q[i].y]=0;
		// 	if(symm)
		// 		tmp_m.m[8-q[i].x][8-q[i].y]=0;
		// }
		a=tmp_m;
		return blank;
	}
public:
	Solver(int lim=2):limit(lim){
		srand(time(0));
		for(int i=1;i<10;i++)idx[1<<i]=i;
	}
	int solve(Mat _,int lim=2){
		ans=tot=0;a=_;limit=lim;
		for(int i=1;i<10;i++)s0[i-1]=s1[i-1]=s2[i-1]=1022;
		for(int i=0;i<9;i++)
			for(int j=0;j<9;j++){
				if(a.m[i][j])add(i,j,pos[i][j],1<<a.m[i][j]);
				showtime.m[i][j]=0;
			}
		for(int i=0;i<9;i++)
			for(int j=0;j<9;j++)
				if(a.m[i][j]==0)p[++tot]=(Trip){i,j,calc(s0[i]&s1[j]&s2[pos[i][j]])};
		std::random_shuffle(p+1,p+1+tot);
		for(int i=1;i<=tot;i++){
			int x=p[i].v,k=i;
			for(int j=i+1;j<=tot;j++)if(p[j].v<x)x=p[j].v,k=j;
			swp=p[i],p[i]=p[k],p[k]=swp;showtime.m[p[i].x][p[i].y]=i;
			for(int j=i+1;j<=tot;j++)if(p[j].x==p[i].x||p[j].y==p[i].y||pos[p[j].x][p[j].y]==pos[p[i].x][p[i].y])p[j].v--;
		}
		dfs(1);
		return ans;
	}
	int generate_range(int l,int r,int symm=0)
	{
		int ans;//=generate(symm);
		while(generate(symm),ans=a.cnt0(),ans<l||ans>r);
		return ans;
	}
}sol;

int main(int argc, char const *argv[])
{
	Mat tmp;
	while(tmp.read()){
		sol.solve(tmp,1);
		sol.a.print();
	}
	return 0;
	double c0=clock();int cnt=0;//srand(1626);
	c0=0;
	for(int i=10000;i--;){
		sol.generate_range(50,81);
//		printf("blank=%d\n",c0+=sol.a.cnt0());
		sol.a.print();
		sol.solve(sol.a);
//		cnt++;printf("%lf\n",c0/cnt);//(clock()-c0)/CLOCKS_PER_SEC/cnt);
		//sol.a.print();
	}
	return 0;
}