#include <stdio.h>
#include <stdlib.h>


typedef struct Root {

    int info;
    struct Root* esquerda;
    struct Root* direita;
    int altura;
    int fatorBalanceamento;

} Root;


Root* initializeRoot(int rootInfo){
    Root* aux=(Root*)malloc(sizeof(Root)); 
    aux->info = rootInfo;
    aux->esquerda = NULL;
    aux->direita = NULL;
    aux->altura = 0;
    aux->fatorBalanceamento = 0;
    return aux;
}

Root* insertRoot(Root* tree, int rootInfo){
    if (tree==NULL) return initializeRoot(rootInfo);
    else if (rootInfo<(tree->info)) {tree->altura+=1; tree->esquerda = insertRoot(tree->esquerda, rootInfo);}
    else {tree->altura+=1; tree->direita =  insertRoot(tree->direita, rootInfo);}
    return tree;

}

Root* findRoot(Root* tree, int rootInfo){

    if (tree==NULL) return tree;
    else if (rootInfo==(tree->info)) return tree;
    else if (rootInfo<(tree->info)) findRoot(tree->esquerda, rootInfo);
    else findRoot(tree->direita, rootInfo);

}

int childrenRoot(Root* tree, int rootInfo){

    Root* aux = findRoot(tree, rootInfo);
    if (aux == NULL) return -1;
    else if (aux->esquerda!=NULL&&aux->direita!=NULL) return 3; 
    else if (aux->esquerda!=NULL) return 2;
    else if (aux->direita!=NULL) return 1;
    else return 0;
}

Root* findParentRoot(Root* tree, Root* root){

    
    if (tree==NULL||root==NULL) return tree;
    else if (root==tree) return NULL;
    else if (root==(tree->esquerda)) return tree;
    else if (root==(tree->direita)) return tree;
    else if (root->info<(tree->info)) return findParentRoot(tree->esquerda, root);
    else return findParentRoot(tree->direita, root);
}

Root* findExtremeRight(Root* tree){
    
    if(tree==NULL) return NULL;
    else{

        Root* aux = tree;
        while(aux->direita!=NULL){
            aux = aux-> direita;
        }

        return aux;
    }
}

void switchRoot(Root** aux1, Root** aux2){

    Root* aux3 = (Root*)malloc(sizeof(Root));

    aux3->info = (*aux1)->info;

    (*aux1)->info = (*aux2)->info;

    (*aux2)->info = aux3->info;

    free(aux3);

}

Root* deleteRoot(Root* tree, int rootInfo){

    Root* parent = findParentRoot(tree, findRoot(tree, rootInfo));
   // printf("parent: %d\n", parent->info);
    if (parent==NULL) return NULL;
    else{

        Root* target = findRoot(tree, rootInfo);
        Root* extreme;
        int children = childrenRoot(tree, rootInfo);
        //printf("children: %d\n", children);
        switch (children){
            case -1:
                printf("nullo\n");
                return NULL;
                break;

            case 0:
               // printf("folha\n");
                if (parent == target){
                    return NULL;
                }
                else if ((parent->esquerda!=NULL)&&(parent->esquerda->info==rootInfo)){

                    parent->esquerda = NULL;
                }
                else{
                    parent->direita = NULL;
                }
                free(target);
                break;

            case 1:
               // printf("raiz com filho na direita\n");
                if ((parent->esquerda!=NULL)&&(parent->esquerda->info==rootInfo)){

                    parent->esquerda = target->direita;
                }
                else{

                    parent->direita = target->direita;
                }
                free(target);
                break;
            
            case 2:
               // printf("raiz com filho na esquerda\n");
                if ((parent->esquerda!=NULL)&&(parent->esquerda->info==rootInfo)){

                    parent->esquerda = target->esquerda;
                }
                else{

                    parent->direita = target->esquerda;
                }
                free(target);
                break;

            case 3: 
               // printf("raiz com 2 filhos\n");
                extreme = findExtremeRight(target->esquerda);
                switchRoot(&target, &extreme);
                //printf("extreme: %d\n", extreme->info);
               // printf("tree: %d\n", tree->info);
               // printf("tree direita: %d\n", tree->direita->info);
               // printf("tree esquerda: %d\n", tree->esquerda->info);
                deleteRoot(tree, extreme->info);
                break;

        }
        return tree;
    }
}

void printTree(Root* tree){
    if (tree==NULL) return;
    else{
	 printf("\nRaiz: %d, Altura: %d, fatorBalanceamento: %d", tree->info, tree->altura, tree->fatorBalanceamento);
      
        if((tree->esquerda!=NULL)&&(tree->direita!=NULL)){
            printf(", Filho esquerda: %d,  Filho direita: %d\n", tree->esquerda->info, tree->direita->info);
        }
        else if(tree->esquerda!=NULL){
            printf(", Filho esquerda: %d,  Filho direita: %d\n", tree->esquerda->info, 0);
        }
        else if(tree->direita!=NULL){
            printf(", Filho esquerda: %d,  Filho direita: %d\n", 0, tree->direita->info);
        }
        else{
             printf(", Filho esquerda: %d,  Filho direita: %d\n", 0, 0);
        }
        printTree(tree->esquerda);
        printTree(tree->direita); 
    }
    return;

}

void updateAltura(Root* tree, int altura){

	if (tree == NULL) return;
	else{
        tree->altura = altura;
        updateAltura(tree->esquerda, altura+1);
        updateAltura(tree->direita, altura+1);
        return;
	}
}

void updateBalancingFactor(Root* tree){
    if (tree == NULL) return;
	else{

        if((tree->esquerda!=NULL)&&(tree->direita!=NULL)){
            tree->fatorBalanceamento = (tree->direita->altura)-(tree->esquerda->altura);
        }
        else if(tree->esquerda!=NULL){
            tree->fatorBalanceamento = (tree->esquerda->altura);
        }
        else if(tree->direita!=NULL){
            tree->fatorBalanceamento = (tree->direita->altura);
        }
        else{
            tree->fatorBalanceamento = 0;
        }
        updateBalancingFactor(tree->esquerda);
        updateBalancingFactor(tree->direita);
	}
}
Root* turnLeft(Root* tree){
    printf("left\n");
    Root* aux = tree->direita;
    tree->direita = aux->esquerda;
    aux->esquerda = tree;
    return aux;
}
Root* turnRight(Root* tree){
    Root* aux = tree->esquerda;
    tree->esquerda = aux->direita;
    aux->direita = tree;
    return aux;
}

Root* Balancing(Root* tree){
    if (tree == NULL) return tree;
    else if (tree->fatorBalanceamento==2){

        if(tree->direita->fatorBalanceamento==1){
            return turnLeft(tree);
        }
        else if(tree->esquerda->fatorBalanceamento==-1){
            return turnRight(tree);
        }
    }
    else if (tree->fatorBalanceamento==-2){
        
        if(tree->direita->fatorBalanceamento==1){
            return turnRight(tree);
        }
        else if(tree->esquerda->fatorBalanceamento==-1){
            return turnLeft(tree);
        }
    }
    else{
        Balancing(tree->esquerda);
        Balancing(tree->direita);
        return tree;
    }   
}
// void showNode(Root* tree){

// 	printf("info: %d, altura: %d, fatorBalanceamento: %d.", tree->info, tree->altura, tree->fatorBalanceamento);

// }

// Root* lookRoot(Root* tree, int rootInfo){

//     if (tree==NULL) return tree;
//     else if (rootInfo==(tree->info))  {showNode(tree); return tree;}
//     else if (rootInfo<(tree->info)) findRoot(tree->esquerda, rootInfo);
//     else findRoot(tree->direita, rootInfo);

// }

int main (){

    Root* tree = NULL;
    int escolha, selecao = 1;

    while(selecao!=0){

        printf("selecione acao: \n1 - inserir \n2 - deletar \n3 - printa arvore \n0 - fechar programa\n");
        scanf("%d", &selecao);
        if(selecao==1){
            printf("escolha valor a ser inserido:\n");
            scanf("%d", &escolha);
            tree = insertRoot(tree, escolha);
        }
        else if(selecao==2){
            printf("escolha valor a ser deletado:\n");
            scanf("%d", &escolha);
            tree = deleteRoot(tree, escolha);
        }
        else if(selecao==3){
        
            updateBalancingFactor(tree);
            tree = Balancing(tree);
            updateAltura(tree, 0);
            updateBalancingFactor(tree);
            printTree(tree);
        }
        else selecao = 0;
    }
    return 0;
}

