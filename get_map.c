/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuendo <yuendo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 17:52:09 by yutoendo          #+#    #+#             */
/*   Updated: 2023/08/23 12:32:43 by yuendo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "so_long.h"

static char	*so_long_strjoin(char *s1, char *s2)
{
	char	*str;
	char	*str_start;
    char *s1_start;
	size_t	size;

	if (!s1 && !s2)
		return (NULL);
    s1_start = s1;
	size = ft_strlen(s1) + ft_strlen(s2) + 1;
	str = (char *)ft_calloc(size, sizeof(char));
	if (!str)
		return (NULL);
	str_start = str;
	while (s1 != NULL && *s1)
		*str++ = *s1++;
	while (s2 != NULL && *s2)
		*str++ = *s2++;
	*str = 0;
	str = str_start;
	free(s1_start);
	return (str_start);
}

static char **read_map(char *map_info)  
{
    int fd; 
    char *line; 
    char *concat_lines;  
    char **map;

    concat_lines = NULL;
    fd = open(map_info, O_RDONLY); 
    if (fd == INVALID_FD)
    {
        ft_printf("\x1b[31mError\nMap not found\n\x1b[0m");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        line = get_next_line(fd);   
        if (line == NULL || line[0] == '\n')    
        {
            break;
        }    
        concat_lines = so_long_strjoin(concat_lines, line);  
        
        if (concat_lines == NULL) 
        {   
            free(line); 
            ft_printf("\x1b[31mError\nMalloc Error\n\x1b[0m");
            exit(EXIT_FAILURE);
        }
        free(line);
    }
    free(line); 
    close(fd);  
    map =  ft_split(concat_lines, '\n');
    free(concat_lines);
    return (map);  
}

static t_img get_img(t_data *game_data, char *path)
{
    t_img element;
    
    element.img = mlx_xpm_file_to_image(game_data->mlx, path, &element.cordinate.x, &element.cordinate.y);  
    return (element);
}

void get_game_imgs(t_data *game_data)
{    
    game_data->player = get_img(game_data, PLAYER_PATH);
    game_data->collectible = get_img(game_data, COINS_PATH);
    game_data->wall = get_img(game_data, BACKGROUND_PATH);
    game_data->exit = get_img(game_data, EXIT_PATH);
    game_data->empty = get_img(game_data, EMPTY_PATH);
    game_data->damage = get_img(game_data, DAMAGE_PATH);
}

static int get_coin_num(char **map)
{
    int i;
    int j;
    int coin_num;

    i = 0;
    coin_num = 0;
    while (map[i] != NULL)
    {
        j = 0;
        while (map[i][j] != '\0')
        {
            if (map[i][j] == 'C')
                coin_num++;
            j++;
        }
        i++;
    }
    return (coin_num);
}


void get_map(char *map_path, t_data *game_data)
{
    char **map;

    map = read_map(map_path);   
    if (map == NULL)   
    {   
        perror("Error\nMalloc Error\n");
        exit(EXIT_FAILURE);
    }
    is_valid_map(map); 
    game_data->map.map = map;   
    game_data->map_width =  ft_strlen(map[0]);
    game_data->map_height = get_map_len(map); 
    game_data->coin_num = get_coin_num(map);  
    is_game_playable(game_data);
}