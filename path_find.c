/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_find.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:25:32 by segunes           #+#    #+#             */
/*   Updated: 2025/06/26 15:57:32 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"

// const char *pathname(char *command)
// {
// 	const char *path;
// 	char **path_env;
// 	const char *join;
// 	const char *temp;
// 	int i;
	
// 	i = 0;
// 	path = getenv("PATH");//getenv ile ortam değişkenlerine erişiyoruz
// 	if(!path)
// 		return (NULL);//erişemezsek null döndürüyoruz
// 	path_env = ft_split(path, ':');//bu değişkenlerde her konuma bakıyoruz 
// 	//"/usr/local/sbin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin" örneğin bu şekilde bir ortam değişkeni aldıysak 
// 	// /usr/local/sbin ilk başta buraya kadar ayırıyoruz
// 	// /usr/local/bin
// 	// /usr/bin
// 	// /bin
// 	// /usr/sbin
// 	// /sbin  bu şekilde hepsini alıp : göre ayırdık
// 	while(path_env[i])
// 	{
// 		//daha sonra her bir konum için bakıyoruz diyelimki /usr/bin bu konumda arayacağız kendi komutumuzu arıyoruz diyedlim ki ben "cat" komutunu arıyorum
// 		//ilk başta /usr/bin/ ile birleştiriyoruz /usr/bin buna "/" eklemiş oluyoruz
// 		// daha sonra
// 		//sonra cat ile birleştiriyoruz
// 		// yani command den gelen komutla onu da biz mainden gönderiyoruz zaten
// 		temp = ft_strjoin(path_env[i], "/");
// 		join = ft_strjoin(temp, command);
// 		//sonra bu birleştirdiğimiz komutun erişilebilir olup olmadığını kontrol ediyoruz
// 		//eğer erişilebilir ise return ediyoruz
// 		//access İlk parametre: kontrol etmek istediğin dosya yolu
// 		//		 İkinci parametre: hangi izinleri kontrol etmek istiyoruz 
// 		// X_OK => dosyanın çalıştırılabilir olup olmadığını kontrol eder (executable ok demek açılımı çalıştırılabilir mi)
// 		if (access(join, X_OK) == 0)
// 			return (join);
// 		else 
// 		{
// 			free((char *)join);
// 			free((char *)temp);
// 		}
// 		i++;
// 	}
// 	return (NULL);
// }

// char *find_path(char *command)
// {
//     char *temp = (char *)pathname(command);
//     if (!temp)
//         return NULL;
//     return ft_strdup(temp); // Böylece belleği dışarıdan yönetebilirsin
// }


void free_string_array(char **array)
{
    int i = 0;
    if (!array)
        return;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}

char *find_path(char *command)
{
    const char *path;
    char **path_env;
    char *temp;
    char *join;
    char *result;
    int i;

    // Önce komutun mutlak yol olup olmadığını kontrol et
    if (command[0] == '/' || command[0] == '.')
    {
        if (access(command, X_OK) == 0)
            return ft_strdup(command);
        return NULL;
    }

    // PATH ortam değişkenini al
    path = getenv("PATH");
    if (!path)
        return NULL;

    path_env = ft_split(path, ':');
    if (!path_env)
        return NULL;

    result = NULL;
    i = 0;
    while (path_env[i] && !result)
    {
        // PATH/command şeklinde birleştir
        temp = ft_strjoin(path_env[i], "/");
        if (!temp)
        {
            i++;
            continue;
        }
        
        join = ft_strjoin(temp, command);
        free(temp); // temp'i hemen serbest bırak
        
        if (!join)
        {
            i++;
            continue;
        }

        // Dosyanın erişilebilir ve çalıştırılabilir olup olmadığını kontrol et
        if (access(join, X_OK) == 0)
        {
            result = ft_strdup(join); // Kopya oluştur
        }
        
        free(join); // join'i her durumda serbest bırak
        i++;
    }

    // path_env dizisini tamamen temizle
    free_string_array(path_env);
    
    return result;
}

// Alternatif olarak, eğer pathname fonksiyonunu kullanmak istiyorsanız:
const char *pathname(char *command)
{
    const char *path;
    char **path_env;
    const char *join;
    const char *temp;
    int i;

    // Mutlak yol kontrolü
    if (command[0] == '/' || command[0] == '.')
    {
        if (access(command, X_OK) == 0)
            return ft_strdup(command);
        return NULL;
    }

    path = getenv("PATH");
    if (!path)
        return NULL;
        
    path_env = ft_split(path, ':');
    if (!path_env)
        return NULL;

    i = 0;
    while (path_env[i])
    {
        temp = ft_strjoin(path_env[i], "/");
        if (!temp)
        {
            i++;
            continue;
        }
        
        join = ft_strjoin(temp, command);
        if (!join)
        {
            free((char *)temp);
            i++;
            continue;
        }

        if (access(join, X_OK) == 0)
        {
            // Başarılı durumda: önce temizlik yap, sonra return et
            free((char *)temp);
            free_string_array(path_env);
            return join; // Bu belleği çağıran fonksiyon yönetecek
        }
        else
        {
            free((char *)join);
            free((char *)temp);
        }
        i++;
    }
    
    // Başarısız durumda: tüm belleği temizle
    free_string_array(path_env);
    return NULL;
}