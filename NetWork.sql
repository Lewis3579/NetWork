--
-- PostgreSQL database dump
--

-- Dumped from database version 17.2
-- Dumped by pg_dump version 17.2

-- Started on 2025-01-20 04:42:33

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET transaction_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- TOC entry 220 (class 1259 OID 16401)
-- Name: files; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.files (
    id integer NOT NULL,
    file_name text,
    file_path text,
    state text,
    owner_id integer,
    folder_id integer,
    file_size bigint
);


ALTER TABLE public.files OWNER TO postgres;

--
-- TOC entry 219 (class 1259 OID 16400)
-- Name: files_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.files_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.files_id_seq OWNER TO postgres;

--
-- TOC entry 4876 (class 0 OID 0)
-- Dependencies: 219
-- Name: files_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.files_id_seq OWNED BY public.files.id;


--
-- TOC entry 222 (class 1259 OID 16410)
-- Name: folders; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.folders (
    id integer NOT NULL,
    folder_name text,
    folder_path text,
    state text,
    owner_id integer,
    parent_id integer
);


ALTER TABLE public.folders OWNER TO postgres;

--
-- TOC entry 221 (class 1259 OID 16409)
-- Name: folders_folder_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.folders_folder_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.folders_folder_id_seq OWNER TO postgres;

--
-- TOC entry 4877 (class 0 OID 0)
-- Dependencies: 221
-- Name: folders_folder_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.folders_folder_id_seq OWNED BY public.folders.id;


--
-- TOC entry 218 (class 1259 OID 16390)
-- Name: users; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.users (
    id integer NOT NULL,
    username text NOT NULL,
    password text NOT NULL
);


ALTER TABLE public.users OWNER TO postgres;

--
-- TOC entry 217 (class 1259 OID 16389)
-- Name: users_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.users_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.users_id_seq OWNER TO postgres;

--
-- TOC entry 4878 (class 0 OID 0)
-- Dependencies: 217
-- Name: users_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.users_id_seq OWNED BY public.users.id;


--
-- TOC entry 4706 (class 2604 OID 16404)
-- Name: files id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.files ALTER COLUMN id SET DEFAULT nextval('public.files_id_seq'::regclass);


--
-- TOC entry 4707 (class 2604 OID 16413)
-- Name: folders id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.folders ALTER COLUMN id SET DEFAULT nextval('public.folders_folder_id_seq'::regclass);


--
-- TOC entry 4705 (class 2604 OID 16393)
-- Name: users id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.users ALTER COLUMN id SET DEFAULT nextval('public.users_id_seq'::regclass);


--
-- TOC entry 4868 (class 0 OID 16401)
-- Dependencies: 220
-- Data for Name: files; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.files (id, file_name, file_path, state, owner_id, folder_id, file_size) FROM stdin;
109	AutoClicker-3.0.exe	D:/dataRoot/Newfolder/AutoClicker-3.0.exe	Public	2	54	864317
110	lonarpg-v0.9.3.0.zip	D:/dataRoot/Newfolder/lonarpg-v0.9.3.0.zip	Public	2	54	461445795
111	Lust_Corruption_of_the_Exorcist_2024-06-29.rar	D:/dataRoot/Newfolder/Lust_Corruption_of_the_Exorcist_2024-06-29.rar	Public	2	54	1126292677
112	NHT.rar	D:/dataRoot/Newfolder/NHT.rar	Public	2	54	157971225
114	RpgMakerSaveEdit_0.69.rar	D:/dataRoot/Newfolder/RpgMakerSaveEdit_0.69.rar	Public	2	54	29697
141	fileINFO.txt	D:/dataRoot/fileINFO.txt	Private	2	0	54
142	Deployed.rar	D:/dataRoot/Newfolder/Deployed.rar	Public	36	54	12921840
143	Employed.rar	D:/dataRoot/Newfolder/Employed.rar	Public	36	54	12921840
144	All.png	D:/dataRoot/All.png	Private	36	0	12590
\.


--
-- TOC entry 4870 (class 0 OID 16410)
-- Dependencies: 222
-- Data for Name: folders; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.folders (id, folder_name, folder_path, state, owner_id, parent_id) FROM stdin;
54	Newfolder	D:/dataRoot/Newfolder	Public	2	0
56	Creasr	D:/dataRoot/Creasr	Public	36	0
0	dataRoot	D:/dataRoot	Public	0	\N
\.


--
-- TOC entry 4866 (class 0 OID 16390)
-- Dependencies: 218
-- Data for Name: users; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.users (id, username, password) FROM stdin;
1	kien	sda
2	khanhdz	toilaai
3	son	ghe
4	Kiemlai	TranBinhAn
5	Phong	Dez
6	tung	cuto
8	tung12	cuto
11	tung421	123
14	trungcuto	anhphuong
16	khanh20	1234
19	lam	khanhs
23	kiendz	trung
0	mod	lana
26	nguyenvana	13572468
29	trungkienson	123
30	doki	ghenti
32	ngaimai	homnay
34	trungkien	toilaai
36	nhtkien	12345
\.


--
-- TOC entry 4879 (class 0 OID 0)
-- Dependencies: 219
-- Name: files_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.files_id_seq', 144, true);


--
-- TOC entry 4880 (class 0 OID 0)
-- Dependencies: 221
-- Name: folders_folder_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.folders_folder_id_seq', 56, true);


--
-- TOC entry 4881 (class 0 OID 0)
-- Dependencies: 217
-- Name: users_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.users_id_seq', 36, true);


--
-- TOC entry 4713 (class 2606 OID 16408)
-- Name: files files_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.files
    ADD CONSTRAINT files_pkey PRIMARY KEY (id);


--
-- TOC entry 4715 (class 2606 OID 16417)
-- Name: folders folders_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.folders
    ADD CONSTRAINT folders_pkey PRIMARY KEY (id);


--
-- TOC entry 4709 (class 2606 OID 16399)
-- Name: users unique_constraint; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT unique_constraint UNIQUE (username);


--
-- TOC entry 4711 (class 2606 OID 16397)
-- Name: users users_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_pkey PRIMARY KEY (id);


--
-- TOC entry 4718 (class 2606 OID 16482)
-- Name: folders fk_child_parent; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.folders
    ADD CONSTRAINT fk_child_parent FOREIGN KEY (parent_id) REFERENCES public.folders(id) ON DELETE CASCADE;


--
-- TOC entry 4716 (class 2606 OID 16472)
-- Name: files fk_files_folders; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.files
    ADD CONSTRAINT fk_files_folders FOREIGN KEY (folder_id) REFERENCES public.folders(id) ON DELETE CASCADE;


--
-- TOC entry 4717 (class 2606 OID 16467)
-- Name: files fk_files_users; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.files
    ADD CONSTRAINT fk_files_users FOREIGN KEY (owner_id) REFERENCES public.users(id) ON DELETE CASCADE;


--
-- TOC entry 4719 (class 2606 OID 16477)
-- Name: folders fk_folders_users; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.folders
    ADD CONSTRAINT fk_folders_users FOREIGN KEY (owner_id) REFERENCES public.users(id) ON DELETE CASCADE;


-- Completed on 2025-01-20 04:42:33

--
-- PostgreSQL database dump complete
--

