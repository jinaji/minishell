/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinkim2 <jinkim2@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 11:16:59 by jinkim2           #+#    #+#             */
/*   Updated: 2022/08/19 20:22:32 by jinkim2          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include <sys/stat.h>



t_key	*make_node(char	*key, char *value)
{
	t_key	*new;

	new = malloc(sizeof(t_key));
	if (!new)
		exit (12);
	new->key = key;
	new->value = value;
	new->next = 0;
	new->prev = 0;
	return (new);
}

char	**mini_split(char *str, char c)
{
	char	**tmp;
	int		len;
	int		i;
	int		j;

	i = 0;
	j = 0;
	len = 0;
	while (str[len] && str[len] != c)
		len++;
	tmp = (char **)malloc(sizeof(char *) * 3);
	if (!tmp)
		exit (12);
	tmp[0] = (char *)malloc(sizeof(char) * len + 1);
	if (!tmp[0])
		exit (12);
	while (i < len)
	{
		tmp[0][i] = str[i];
		i++;
	}
	tmp[0][i] = 0;
	len = ft_strlen(str) - len;
	tmp[1] = (char *)malloc(sizeof(char) * len + 1);
	if (!tmp[1])
		exit (12);
	while (str[i])
		tmp[1][j++] = str[++i];
	tmp[2] = 0;
	return (tmp);
}

void	make_envlist(t_key	**env, char **dup_envp)
{
	t_key	*temp;
	t_key	*new;
	char	**tmp;
	int		i;

	i = 0;
	(*env) = malloc(sizeof(t_key));
	(*env)->head = (*env);
	temp = *env;
	while (dup_envp[i])
	{
		tmp = ft_split(dup_envp[i], '=');
		if (!tmp)
			exit (12);
		new = make_node(tmp[0], tmp[1]);
		(*env)->tail = new;
		temp->next = new;
		new->prev = temp;
		temp = temp->next;
		i++;
	}
	temp = (*env)->head;
	(*env)->head = temp->next;
	temp->next = 0;
	temp->prev = 0;
	free (temp);
}

int	ft_strcmp(char *str1, char *str2)
{
	int	i;

	i = 0;
	while (str1[i])
	{
		if (str1[i] != str2[i])
			return (0);
		i++;
	}
	if (str2[i])
		return (0);
	return (1);
}

void	print_export(t_key *env)
{
	t_key	*tmp;

	tmp = env->head;
	while (tmp)
	{
		if ((ft_strcmp(tmp->key, "_")))
			;
		else if (ft_strcmp(tmp->value, "donotprint"))
			printf("declare -x %s\n", tmp->key);
		else
			printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
}


void	print_cd_err(struct stat *buf, char *str)
{
	lstat(str, buf);
	// printf ("%hu\n", buf->st_mode);
	if (buf->st_mode == 0)
		printf ("shellname: cd: %s: No such file or directory\n", str);
	if (buf->st_mode == 16384)
		printf ("shellname: cd: %s: Permission denied\n", str);
	if (buf->st_mode == 33261)
		printf("shellname: cd: %s: Not a directory\n", str);
}

char	*ft_getenv(t_key *env, char *str)
{
	t_key	*tmp;

	tmp = env->head;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, str))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (0);
}

// char	*key_exist(char *str, t_key *env)
// {
// 	t_key	*tmp;

// 	tmp = env->head;
// 	while (tmp)
// 	{
// 		if (ft_strcmp(tmp->key, str))
// 			return (tmp->value);
// 		tmp = tmp->next;
// 	}
// 	return (0);
// }

int	go_home(char *str, t_key *env)
{
	if (!str || ft_strcmp(str, "HOME") || ft_strcmp(str, "~") || !ft_getenv(env, str))
	{
		if (!ft_getenv(env, "HOME"))
			printf("HOME not set\n");
		else
			chdir(ft_getenv(env, "HOME"));
	}
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	t_key		*env;
	struct stat	*buf;
	int			ret;

	ret = 1;
	buf = malloc(sizeof(stat));
	make_envlist(&env, envp);
	if (ac == 1 || av[1] && go_home(av[1], env))
		ret = chdir(ft_getenv(env, "HOME"));
	else if (ft_strcmp(av[1], "-"))
		ret = chdir(ft_getenv(env, "OLDPWD"));
	else
	{
		printf("%s\n", av[1]);
		ret = chdir(av[1]);
		int ret2= chdir("d");
		printf("ret2 %d\n", ret2);
	}
	if (ret == -1)
		print_cd_err(buf, av[1]);
	free (buf);
	// printf("%d\n", ret);
	// printf ("%s\n", av[1]);
	system("pwd");
}
