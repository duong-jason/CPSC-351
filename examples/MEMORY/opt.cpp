// https://github.com/pockemon/Operating-systems--LAB/blob/master/LAB9/optimal.cpp

#include <bits/stdc++.h>
#include <iostream>

int present(int table_frame[], int nf, int page) {
	for (int i = 0; i < nf; i++)
		if (page == table_frame[i]) return 1;
	return 0;
}

void printtable(int table_frame[], int nf) {
	for (int i=0; i < nf; i++) {
		if (table_frame[i] == -1)
			printf("-- ");
		else
			printf("%2d ", table_frame[i]);
	}
	printf("||");
}

int findpos(int table_frame[],int nf,int pages[],int curr,int np) {
    int i, j;
    for (i = 0; i < nf; i++) {
        if (table_frame[i] == -1) return i;
    }

    int pos[nf]={0};
    for (i = 0; i < nf; i++) {
        pos[i] = 1e9;
        for (j = curr+1; j < np; j++) {
            if (pages[j] == table_frame[i]) {
				pos[i]=j;
				break;
            }
        }
    }

    int max1 = -1;
    int returnpos = -1;
    for (i = 0;i < nf; i++) {
        if (pos[i] > max1) {
            max1 = pos[i];
            returnpos = i;
        }
    }

    return returnpos;
}

int main() {
    // nf-number of frames
    int n, nf, i, pos = 0;

    printf("Enter number of frames: ");
    scanf("%d",&nf);
    int table_frame[nf];
    for(i = 0; i < nf; i++) table_frame[i]=-1;

    printf("Enter total number of page requests: ");
    scanf("%d",&n);
    int pages[n];
    printf("Enter pages: ");
    for (i = 0;i < n; i++) scanf("%d",&pages[i]);

    int count1 = 0;
	std::cout << "\n\n";
    for (i = 0; i < n; i++) {
        if (!present(table_frame, nf, pages[i])) {
			printf("page table after request from %2d || ", pages[i]);
            int pos = findpos(table_frame, nf, pages, i, n);
            table_frame[pos] = pages[i];

            printtable(table_frame, nf);
			printf("page fault\n");
			count1++;
            continue;
        }
    }
    printf("\nNumber of page faults: %d\n\n", count1);
}
